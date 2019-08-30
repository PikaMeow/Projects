import torch
import torch.nn as nn
from torch.nn import Parameter


class ScratchLSTM(nn.Module):
    def __init__(self, input_sz=300, hidden_sz=300):
        super().__init__()
        self.input_size = input_sz
        self.hidden_size = hidden_sz
        self.weight_ih = Parameter(torch.Tensor(input_sz, hidden_sz * 4))
        self.weight_hh = Parameter(torch.Tensor(hidden_sz, hidden_sz * 4))
        self.bias = Parameter(torch.Tensor(hidden_sz * 4))
        self.init_weights()

    def init_weights(self):
        for p in self.parameters():
            if p.data.ndimension() >= 2:
                nn.init.xavier_uniform_(p.data)
            else:
                nn.init.zeros_(p.data)

    def forward(self, x, hidden_states):
        """Assumes x is of shape (batch, sequence, feature)"""
        x = x.transpose(0, 1).contiguous()

        bs, seq_sz, _ = x.size()
        hidden_seq = []
        if hidden_states is None:
            h_t, c_t = (torch.zeros(self.hidden_size).to(x.device),
                        torch.zeros(self.hidden_size).to(x.device))
        else:
            h_t, c_t = hidden_states

        HS = self.hidden_size
        for t in range(seq_sz):
            x_t = x[:, t, :]
            # batch the computations into a single matrix multiplication
            gates = x_t @ self.weight_ih + h_t @ self.weight_hh + self.bias # @:torch中的矩阵乘法

            i_t = torch.sigmoid(gates[:, :HS])  # input
            f_t = torch.sigmoid(gates[:, HS:HS * 2])  # forget
            g_t = torch.tanh(gates[:, HS * 2:HS * 3])
            o_t = torch.sigmoid(gates[:, HS * 3:])  # output

            c_t = f_t * c_t + i_t * g_t
            h_t = o_t * torch.tanh(c_t)
            hidden_seq.append(h_t.unsqueeze(0))
        hidden_seq = torch.cat(hidden_seq, dim=0).contiguous()
        return hidden_seq, (h_t, c_t)


class RNNModel(nn.Module):
    """ 一个简单的循环神经网络"""

    def __init__(self, rnn_type, ntoken, ninp, nhid, nlayers, dropout=0.5):
        ''' 该模型包含以下几层:
            - 词嵌入层
            - 一个循环神经网络层(RNN, LSTM, GRU)
            - 一个线性层，从hidden state到输出单词表
            - 一个dropout层，用来做regularization
        '''
        super(RNNModel, self).__init__()
        self.drop = nn.Dropout(dropout)
        self.encoder = nn.Embedding(ntoken, ninp)

        self.rnn = ScratchLSTM(ninp, nhid)

        self.decoder = nn.Linear(nhid, ntoken)

        self.init_weights()

        self.rnn_type = rnn_type
        self.nhid = nhid  # hidden size
        self.nlayers = nlayers

    def init_weights(self):
        initrange = 0.1
        self.encoder.weight.data.uniform_(-initrange, initrange)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-initrange, initrange)

    def forward(self, input, hidden):
        ''' Forward pass:
            - word embedding
            - 输入循环神经网络
            - 一个线性层从hidden state转化为输出单词表
        '''
        # text:seq_length * batch_size
        emb = self.drop(self.encoder(input))  # seq_length * batch_size * embed_size
        output, hidden = self.rnn(emb, hidden)
        # output:seq_length * batch_size * hidden
        # hidden:(1 * batch_size * hidden_size, 1 * batch_size * hidden_size)
        # output: (seq_length * batch_size) * hidden_size
        output = self.drop(output)
        out_vocab = self.decoder(output.view(output.size(0) * output.size(1), output.size(2)))
        # out_vocab: (seq_length * batch_size) * vocab_size
        return out_vocab.view(output.size(0), output.size(1), out_vocab.size(1)), hidden

    def init_hidden(self, bsz, requires_grad=True):
        weight = next(self.parameters())
        return (weight.new_zeros((bsz, self.nhid), requires_grad=requires_grad),
                    weight.new_zeros((bsz, self.nhid), requires_grad=requires_grad))