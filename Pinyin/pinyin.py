# -*- coding: utf-8 -*-  

import json
import sys


def get_charset():
    charset = {}
    # 汉字表里汉字的Unicode编码范围是[ u4e00 , u9fa0 ]
    with open('./data/一二级汉字表.txt', encoding='gbk') as f:
        for i in f.read():
            charset[i] = {'count': 0, 'pairs': 0}
        charset['len'] = 0
    return charset


def get_corpus_weibo():
    corpus = []
    with open('../../../../weibo.txt', encoding='utf-8') as f:
        for line in f:
            line = line.strip('\n').split('\t')
            if len(line) != 25:
                continue
            txt = line[-7]
            if '回复' in txt or '转发' in txt or '//@' in txt:
                continue
            corpus.append(txt)
    return corpus


def get_corpus_wiki():
    corpus = []
    with open('../../../../wiki_00', encoding='utf-8') as f:
        for line in f:
            line = line.strip('\n')
            if len(line) == 0 or 'doc' in line:
                continue
            corpus.append(line)
    return corpus


def get_corpus_news():
    corpus = []
    for path in ['02', '04', '05', '06', '07', '08', '09', '10', '11']:
        with open('../../../../sina_news_gbk/2016-' + path + '.txt', encoding='gbk') as f:
            for line in f:
                corpus.append((json.loads(line))['html'])
    for path in ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14']:
        with open('../../../../NLPIR-news-corpus/' + path + '.txt', encoding='utf-8') as f:
            for line in f:
                line = line.strip('\n\t ')
                if len(line) == 0:
                    continue
                corpus.append(line)
    return corpus


def handle_corpus(corpus, charset, cplen):
    for stat in corpus:
        prev = ''
        for char in stat:
            if u'\u4e00' <= char <= u'\u9fa5':
                if char in charset:
                    cplen += 1
                    charset[char]['count'] += 1
                    if prev != '':
                        if prev in charset[char]:
                            charset[char][prev] += 1
                        else:
                            charset[char][prev] = 1
                        charset[char]['pairs'] += 1
                    prev = char
                else:
                    prev = ''
            else:
                prev = ''
    return cplen


def train_charset():
    charset = get_charset()
    corpus = []
    corpus += get_corpus_weibo()
    corpus += get_corpus_wiki()
    corpus += get_corpus_news()
    cplen = handle_corpus(corpus, charset, charset['len'])
    charset['len'] = cplen
    return charset


def dump_charset():
    charset = train_charset()
    with open('../src/model.json', 'w', encoding='utf-8') as f:
        json.dump(charset, f)


def load_charset(model_path):
    with open(model_path, 'r', encoding='utf-8') as f:
        charset = json.load(f)
    return charset


def evaluate(char1, char2, t):  # 得到概率P(char1|char2)，t为平滑参数
    if char1 in charset[char2]:
        p2 = charset[char2][char1]
        p21 = p2 / charset[char1]['count']
    else:
        p21 = charset[char2]['count'] / charset['len'] * (1 - t)
    return p21


def dp(chars, charset, candidate, t):
    prob = {0: {}}
    for start in candidate[chars[0]]:
        prob[0][start] = [charset[start]['count'], '']
    idx = 1
    for char in chars[1:]:
        prob[idx] = {}
        for cand in candidate[char]:
            for key in prob[idx - 1]:
                cur_pb = prob[idx - 1][key][0] * evaluate(key, cand, t)
                if cand not in prob[idx] or (cand in prob[idx] and prob[idx][cand][0] < cur_pb):
                    prob[idx][cand] = [cur_pb, key]  # 仅保留概率积最大的结果
        idx += 1
    idx -= 1

    # backtrace 回溯获取对应的汉字语句 
    max_prob = -1
    item = ''
    for cand in prob[idx]:
        if max_prob < prob[idx][cand][0]:
            max_prob = prob[idx][cand][0]
            item = cand
    stat = [item]
    while item != '':
        item = prob[idx][item][1]
        stat.insert(0, item)
        idx -= 1
    return ''.join(stat)


def accuracy():
    f1 = open('../data/output.txt', 'r', encoding='utf-8')
    f2 = open('../data/groundtruth.txt', 'r', encoding='utf-8')
    lines1 = f1.read().strip('\n').split('\n')
    lines2 = f2.read().strip('\n').split('\n')
    twcnt = 0
    tllen = 0
    lno = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    for line1, line2 in zip(lines1, lines2):
        wcnt = 0
        llen = len(line1)
        for char1, char2 in zip(line1, line2):
            if char1 != char2:
                wcnt += 1
        aur = (llen - wcnt) / llen
        if aur == 0:
            lno[0] += 1
        elif aur == 1:
            lno[10] += 1
        else:
            lno[int(aur * 10)] += 1
        twcnt += wcnt
        tllen += llen
    print("Total accuracy: ", (tllen - twcnt) / tllen)
    print(
        "  准确度 ：  0  [0.1,0.2)  [0.2,0.3)  [0.3,0.4)  [0.4,0.5)  [0.5,0.6)  [0.6,0.7)  [0.7,0.8)  [0.8,0.9)  [0.9,1.0)  1")
    lno_str = [str(x) for x in lno]
    print('  语句数 ： ' + '        '.join(lno_str))
    f1.close()
    f2.close()


if __name__ == '__main__':
    # 处理命令行参数
    input_path = sys.argv[1]
    output_path = sys.argv[2]

    # 载入语料库
    charset = load_charset('model_weibo_wiki.json')  # 可更改此处选择所使用的语料模型

    # 生成并预处理全拼候选汉字表
    candidate = {}
    with open('../data/拼音汉字表.txt', 'r', encoding='gbk') as f:
        for line in f:
            line = line.strip('\n').split(' ')
            candidate[line[0]] = line[1:]
        candidate['qv'] = candidate['qu']
        candidate['xv'] = candidate['xu']
    for char in candidate:
        candidate[char].sort(key=lambda x: charset[x]['count'], reverse=True)

    # 处理全拼语句并生成汉字语句
    t = 0.83  # 此处可以修改平滑参数
    with open(input_path, 'r', encoding='utf-8') as fin:
        with open(output_path, 'w', encoding='utf-8') as fout:
            for line in fin:
                line = line.lower().strip('\n ').split(' ')
                stat = dp(line, charset, candidate, t)
                fout.write(stat + '\n')
    accuracy()
