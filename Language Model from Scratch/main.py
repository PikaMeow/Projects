import torchtext
import torch
import torch.nn as nn
import numpy as np
import random

import model

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# 固定random seed，以保证实验结果可复现
random.seed(53113)
np.random.seed(53113)
torch.manual_seed(53113)

if torch.cuda.is_available():
    torch.cuda.manual_seed(53113)

BATCH_SIZE = 64
EMBEDDING_SIZE = 650
MAX_VOCAB_SIZE = 50000

# 使用 torchtext 读入数据，建立字典
TEXT = torchtext.data.Field(lower=True)
train, val, test = torchtext.datasets.LanguageModelingDataset.splits(path="../..",
                                                                     train="data/ptb/train.txt",
                                                                     validation="data/ptb/valid.txt",
                                                                     test="data/ptb/test.txt",
                                                                     text_field=TEXT)
TEXT.build_vocab(train, max_size=MAX_VOCAB_SIZE)
print("vocabulary size: {}".format(len(TEXT.vocab)))

VOCAB_SIZE = len(TEXT.vocab)
train_iter, val_iter, test_iter = torchtext.data.BPTTIterator.splits(
    (train, val, test), batch_size=BATCH_SIZE, device=device, bptt_len=32, repeat=False, shuffle=True)

# 读入GloVe的向量字典
glove = model.GloveEmbeddings('glove_dict.pkl')

# 根据 rnn_type 建立模型
rnn_type = "LSTM"
lmmodel = model.RNNModel(rnn_type, VOCAB_SIZE, EMBEDDING_SIZE, EMBEDDING_SIZE + glove.glove_dim, 2, dropout=0.5)
lmmodel = lmmodel.to(device)


def repackage_hidden(h):
    """Wraps hidden states in new Tensors, to detach them from their history."""
    if isinstance(h, torch.Tensor):
        return h.detach()
    else:
        return tuple(repackage_hidden(v) for v in h)



def gen_glove(data):
    dim0 = data.size(0)
    data = data.view(-1)
    tokens = [TEXT.vocab.itos[x.item()] for x in data]
    glove_emb = glove.get_embeddings(tokens)
    glove_emb = torch.FloatTensor(glove_emb)
    return glove_emb.view(dim0, -1, glove.glove_dim).contiguous()


loss_fn = nn.CrossEntropyLoss()
learning_rate = 0.0008
optimizer = torch.optim.Adam(lmmodel.parameters(), lr=learning_rate)
scheduler = torch.optim.lr_scheduler.ExponentialLR(optimizer, 0.3)

val_accr = [] # 记录训练过程中准确性的变化

def evaluate(lmmodel, data):
    lmmodel.eval()
    total_loss = 0.
    it = iter(data)
    total_count = 0.
    total_accr = 0
    total_word = 0
    with torch.no_grad():
        hidden = lmmodel.init_hidden(BATCH_SIZE)
        for i, batch in enumerate(it):
            data, target = batch.text, batch.target
            glove_emb = gen_glove(data).to(device)
            data, target = data.to(device), target.to(device)
            hidden = repackage_hidden(hidden)
            with torch.no_grad():
                output, hidden = lmmodel(data, glove_emb, hidden)
            output_flat = output.view(-1, VOCAB_SIZE)
            target = target.view(-1)
            total_word += target.size(0)
            preds = torch.argmax(output_flat, dim=1)
            cur_preds = torch.eq(preds, target)
            total_accr += cur_preds.sum().item()
            loss = loss_fn(output_flat, target)
            total_count += np.multiply(*data.size())
            total_loss += loss.item() * np.multiply(*data.size())
    accr = total_accr / total_word
    print("Eval accr: " + str(accr))
    val_accr.append(accr)
    loss = total_loss / total_count
    lmmodel.train()
    return loss


GRAD_CLIP = 1.
NUM_EPOCHS = 50
train_losses = []
val_losses = []

for epoch in range(NUM_EPOCHS):
    lmmodel.train()
    it = iter(train_iter)
    hidden = lmmodel.init_hidden(BATCH_SIZE)
    for i, batch in enumerate(it):
        data, target = batch.text, batch.target
        glove_emb = gen_glove(data).to(device)
        data, target = data.to(device), target.to(device)
        hidden = repackage_hidden(hidden)
        lmmodel.zero_grad()
        output, hidden = lmmodel(data, glove_emb, hidden)
        loss = loss_fn(output.view(-1, VOCAB_SIZE), target.view(-1))
        loss.backward()
        torch.nn.utils.clip_grad_norm_(lmmodel.parameters(), GRAD_CLIP)
        optimizer.step()

        if i % 1000 == 0:
            print("epoch", epoch, "iter", i, "loss", loss.item())
            train_losses.append(loss.item())

    val_loss = evaluate(lmmodel, val_iter)
    print("best model, val loss: ", val_loss)
    print("perplexity: ", np.exp(val_loss))
    val_losses.append(val_loss)

    # 保存Loss最小的模型
    if val_loss <= min(val_losses):
        torch.save(lmmodel.state_dict(), rnn_type + "_Best.th")
        print("best model saved!")
    else:
        scheduler.step()
        optimizer = torch.optim.Adam(lmmodel.parameters(), lr=learning_rate)


# 测试Loss最小的模型的性能
best_model = model.RNNModel(rnn_type, VOCAB_SIZE, EMBEDDING_SIZE, EMBEDDING_SIZE + glove.glove_dim, 2, dropout=0.5)

if torch.cuda.is_available():
    best_model = best_model.to(device)
best_model.load_state_dict(torch.load(rnn_type + "_Best.th"))
val_loss = evaluate(best_model, val_iter)
print("perplexity: ", np.exp(val_loss))

