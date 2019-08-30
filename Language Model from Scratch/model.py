import torch
import numpy as np
import torch.nn as nn
import pickle


class GloveEmbeddings(object):
    '''
    Class to get the glove embeddings of the words

    Arguments:
        glove_dim : dimension of the glove embedding

    Returns:
        None
    '''

    def __init__(self, file, glove_dim=300):
        with open(file, 'rb') as f:
            self.glove = pickle.load(f)
        self.glove_dim = glove_dim

    '''
    Get the glove embedding of the specified tokens

    Arguments:
        tokens : words whose glove embedding is required

    Returns:
        vectors : glove embedding of the specified tokens in vector form
    '''

    def get_embeddings(self, tokens):
        vectors = []
        for token in tokens:
            token = token.lower().replace("\'s", "")
            if token in self.glove:
                vectors.append(np.array(self.glove[token]))
            else:
                vectors.append(np.zeros((self.glove_dim,)))
        return vectors


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
        # 300 为 glove_dim 的大小
        if rnn_type in ['LSTM', 'GRU']:
            self.rnn = getattr(nn, rnn_type)(ninp + 300, nhid, nlayers, dropout=dropout)
        else:
            try:
                nonlinearity = {'RNN_TANH': 'tanh', 'RNN_RELU': 'relu'}[rnn_type]
            except KeyError:
                raise ValueError("""An invalid option for `--model` was supplied,
                                 options are ['LSTM', 'GRU', 'RNN_TANH' or 'RNN_RELU']""")
            self.rnn = nn.RNN(ninp + 300, nhid, nlayers, nonlinearity=nonlinearity, dropout=dropout)
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

    def forward(self, input, glove_emb, hidden):
        ''' Forward pass:
            - word embedding
            - 输入循环神经网络
            - 一个线性层从hidden state转化为输出单词表
        '''
        # text:seq_length * batch_size
        emb = self.drop(self.encoder(input))  # seq_length * batch_size * embed_size
        emb = torch.cat([emb, glove_emb], dim=2)
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
        if self.rnn_type == 'LSTM':
            return (weight.new_zeros((self.nlayers, bsz, self.nhid), requires_grad=requires_grad),
                    weight.new_zeros((self.nlayers, bsz, self.nhid), requires_grad=requires_grad))  # LSTM返回两个
        else:
            return weight.new_zeros((self.nlayers, bsz, self.nhid), requires_grad=requires_grad)
