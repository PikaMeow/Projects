import torch
import torch.nn as nn


class Attention(nn.Module):

    def __init__(self, seq_len=18, hidden_emb=1024):
        super(Attention, self).__init__()

        self.seq_len = seq_len
        self.hidden_emb = hidden_emb
        self.mlp1_units = 2000
        self.mlp2_units = hidden_emb

        self.fc = nn.Sequential(
            # nn.Linear(self.seq_len * self.hidden_emb, self.mlp1_units),
            # nn.ReLU(inplace=True),
            # nn.Dropout(p=0.5),
            nn.Linear(self.seq_len * self.hidden_emb, self.mlp2_units),
            nn.ReLU(inplace=True),
            nn.Dropout(p=0.5),
            nn.Linear(self.mlp2_units, self.seq_len),
            nn.ReLU(inplace=True),
        )

        self.softmax = nn.Softmax(dim=1)

    '''
    Computes the attention on the lstm outputs from each time step in the sequence

    Arguments:
        lstm_emd : lstm embedding from each time step in the sequence

    Returns:
        attn_feature_map : embedding computed after applying attention to the lstm embedding of the entire sequence
    '''

    def forward(self, lstm_emd):
        lstm_emd = lstm_emd.transpose(0, 1).contiguous()
        direct_emd = lstm_emd[:, :self.seq_len - 1]
        batch_size = lstm_emd.shape[0]

        # 对除了前 seq_len 个词之外的词，都要对其与相邻的之前 seq_len 个词一起输入 Attention 子网络
        for i in range(lstm_emd.shape[1] - self.seq_len + 1):
            cur_lstm = lstm_emd[:, i:i + self.seq_len]
            lstm_flattened = cur_lstm.view(batch_size, -1)
            attn = self.fc(lstm_flattened)
            alpha = self.softmax(attn)
            alpha = torch.stack([alpha] * self.mlp2_units, dim=2)
            attn_feature_map = cur_lstm * alpha
            attn_feature_map = torch.sum(attn_feature_map, dim=1, keepdim=True)
            direct_emd = torch.cat((direct_emd, attn_feature_map), dim=1)

        return direct_emd.transpose(0, 1).contiguous()


class RNNModel(nn.Module):
    """Container module with an encoder, a recurrent module, and a decoder."""

    # model.RNNModel(args.model, ntokens, args.emsize, args.nhid, args.nlayers, args.dropout, args.tied).to(device)
    # ntoken - 不同的词的数目      ninp - embedding向量的维度
    # nhid - 隐藏层单元的维度       nlayers - 网络的层数
    def __init__(self, rnn_type, ntoken, ninp, nhid, nlayers, dropout=0.5, tie_weights=False):
        super(RNNModel, self).__init__()
        self.drop = nn.Dropout(dropout)
        self.encoder = nn.Embedding(ntoken, ninp)
        if rnn_type in ['LSTM', 'GRU']:
            self.rnn = getattr(nn, rnn_type)(ninp, nhid, nlayers, dropout=dropout, bidirectional=False)
        else:
            try:
                nonlinearity = {'RNN_TANH': 'tanh', 'RNN_RELU': 'relu'}[rnn_type]
            except KeyError:
                raise ValueError("""An invalid option for `--model` was supplied,
                                 options are ['LSTM', 'GRU', 'RNN_TANH' or 'RNN_RELU']""")
            self.rnn = nn.RNN(ninp, nhid, nlayers, nonlinearity=nonlinearity, dropout=dropout)
        self.decoder = nn.Linear(nhid, ntoken)

        self.attention = Attention(8, nhid)

        self.init_weights()

        self.rnn_type = rnn_type
        self.nhid = nhid
        self.nlayers = nlayers

    def init_weights(self):
        initrange = 0.1
        self.encoder.weight.data.uniform_(-initrange, initrange)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-initrange, initrange)
        for layer in self.attention.fc:
            if isinstance(layer, nn.Linear):
                layer.weight.data.uniform_(-initrange, initrange)
                layer.bias.data.zero_()

    def forward(self, input, hidden):
        emb = self.drop(self.encoder(input))
        output, hidden = self.rnn(emb, hidden)
        output = self.attention(output)
        output = self.drop(output)
        decoded = self.decoder(output.view(output.size(0) * output.size(1), output.size(2)))
        return decoded.view(output.size(0), output.size(1), decoded.size(1)), hidden

    def init_hidden(self, bsz):
        weight = next(self.parameters())
        if self.rnn_type == 'LSTM':
            return (weight.new_zeros(self.nlayers, bsz, self.nhid),
                    weight.new_zeros(self.nlayers, bsz, self.nhid))
        else:
            return weight.new_zeros(self.nlayers, bsz, self.nhid)
