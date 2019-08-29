#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

#define MAXN 200

int a[MAXN];     //存储每个工件的工序数目
int pos_1[MAXN]; //临时保存某条染色体的前面一部分序列，用于交叉互换
int pos_2[MAXN];
int tmp_1[MAXN]; //临时保存某条染色体的序列，用于交叉互换
int tmp_2[MAXN];
int s[MAXN][MAXN];   //存储每条染色体每个位置上的值
int tmp[MAXN][MAXN]; //临时存储被选中到下一代的染色体
struct det {
    int m;
    int t;
};                        //表示工序所用机器编号和所需时间的结构体
struct det p[MAXN][MAXN]; //存储各工件各工序所用机器和时间的信息
int pt[MAXN][MAXN];       //从开始加工直到加工完某工件某工序的时间
int ma[MAXN];             //存储机器在加工完某些工序时耗费的时间
int cnt[MAXN];            //存储某个工件加工到了第几道工序
int total[MAXN];          //存储每条染色体的适应值
double suit[MAXN];        //存储每条染色体的适应度值
double prob[MAXN];        //存储每条染色体可能发生变异或被选中的概率
bool chos[MAXN];          //某条染色体是否被选中

void produce(int a[], int n, int len, int m) {
    //n为工件个数，len为总的工序数目，m为种群规模(即要产生多少条染色体)
    memset(s, 0, sizeof(s));
    for (int i = 1; i <= m; i++) {
        srand((int)time(0) + rand());
        for (int j = 1; j <= n; j++) {
            int tmp = 0;
            while (tmp != a[j]) {
                int index = rand() % len + 1;
                if (!s[i][index]) {
                    s[i][index] = j;
                    tmp++;
                } //表示第i条染色体的第index个位置上正在对工件j进行加工
            }
        }
    }
}

void printa(int a[], int n) {
    for (int i = 1; i <= n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

void printa(double a[], int n) {
    for (int i = 1; i <= n; i++)
        printf("%.3f ", a[i]);
    printf("\n");
}

void printa(int a[][MAXN], int num, int len) {
    for (int i = 1; i <= num; i++) {
        for (int j = 1; j <= len; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void cal_time(struct det p[][MAXN], int a[], int n, int s[][MAXN], int len, int pos, int total[]) {
    //n为工件个数，len为总的工序数目，pos为第几条染色体
    memset(ma, 0, sizeof(ma));
    memset(pt, 0, sizeof(pt));
    for (int i = 1; i <= n; i++)
        cnt[i] = 1;
    for (int i = 1; i <= len; i++) {
        int part      = s[pos][i];                              //正在加工第几个工件
        int seq       = cnt[part];                              //加工到了第几道工序
        int mac       = p[part][seq].m;                         //工件工序所用的机器
        int time      = p[part][seq].t;                         //工件工序所用的时间
        pt[part][seq] = max(ma[mac], pt[part][seq - 1]) + time; //加工完这个工序所需要的时间
        ma[mac]       = pt[part][seq];                          //加工完当前工序所占有的机器消耗的时间
        if (seq == a[part])
            if (total[pos] < pt[part][seq])
                total[pos] = pt[part][seq]; //加工完整个过程的时间
        //printf("%d %d %d %d %d %d \n",part,seq,mac,time,pt[part][seq],ma[mac]);
        cnt[part]++;
    }
}

void cal_suit(int n, int s[][MAXN], int len, int pos, int total[]) {
    //n为工件个数，len为总的工序数目，pos为第几条染色体
    cal_time(p, a, n, s, len, pos, total);
    suit[pos] = 1.0 / total[pos];
}

void duplicate(int n, int s[][MAXN], int len, int tot, int m, int total[]) {
    //printa(total,tot);
    //printa(suit,tot);
    double sum = 0;
    for (int i = 1; i <= tot; i++) {
        sum += suit[i];
    }
    memset(chos, 0, sizeof(chos));
    prob[0] = 0;
    for (int i = 1; i <= tot; i++)
        prob[i] = suit[i] / sum;
    for (int i = 1; i <= tot; i++) {
        prob[i] += prob[i - 1];
    }
    //printa(prob,tot);
    srand((int)time(0) + rand());
    int cnt        = 0;
    double average = sum * 1.3 / tot;
    //printf("%.4f\n",average);
    //精英保留策略，适应度值大于average的染色体都被保留下来
    for (int i = 1; i <= tot && cnt < m; i++) {
        if (suit[i] >= average) {
            chos[i] = true;
            cnt++;
        }
    }
    //轮盘赌策略，根据概率选择要保留的染色体
    while (cnt < m) {
        double ran = rand() * 1.0 / RAND_MAX;
        //printf("%.4f ",ran);
        for (int i = 1; i <= tot; i++) {
            if (ran >= prob[i - 1] && ran < prob[i]) {
                if (chos[i] == 0) {
                    chos[i] = true;
                    cnt++;
                    break;
                }
            }
        }
    }

    cnt = 0;
    //for(int i=1;i<=tot;i++) if(chos[i]) {printa(s[i],len);printf("%d\n",total[i]);}
    //printf("\n");
    memset(tmp, 0, sizeof(tmp));

    for (int i = 1; i <= tot && cnt < m; i++) {
        if (chos[i]) {
            cnt++;
            for (int j = 1; j <= len; j++) {
                tmp[cnt][j] = s[i][j];
            }
        }
    }
    //printa(tmp,m,len);
    memset(s, 0, sizeof(s));
    //memcpy(s,tmp,sizeof(tmp));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= len; j++) {
            s[i][j] = tmp[i][j];
        }
    }
    //printa(s,m,len);
    for (int i = 1; i <= m; i++)
        total[i] = 0;
    //memset(total,0,sizeof(total));
    //printa(total,m);
    for (int i = 1; i <= m; i++) {
        //cal_time(p,a,n,s,len,i);
        cal_suit(n, s, len, i, total);
    }
    //printa(total,m);
    //printa(suit,m);
}

void crossover(int s[][MAXN], int len, int pos1, int pos2, int &tot) {
    //pos1和pos2为两个参与交换的染色体，tot为当前种群的规模
    srand((int)time(0) + rand());
    for (int i = 1; i <= len; i++) {
        tmp_1[i] = s[pos1][i];
        tmp_2[i] = s[pos2][i];
    }
    int ran = rand() % len;
    for (int i = 1; i <= ran; i++) {
        pos_1[i] = tmp_1[i];
        pos_2[i] = tmp_2[i];
    }
    int cnt = 1;
    while (cnt <= ran) {
        for (int i = 1; i <= len && cnt <= ran; i++) {
            if (pos_2[cnt] == tmp_1[i]) {
                tmp_1[i] = 0;
                cnt++;
            }
        }
    } //从头往后找与另一条相匹配的序列，没有找完就从头接着找

    cnt = 1;
    while (cnt <= ran) {
        for (int i = 1; i <= len && cnt <= ran; i++) {
            if (pos_1[cnt] == tmp_2[i]) {
                tmp_2[i] = 0;
                cnt++;
            }
        }
    }

    cnt = ran + 1;
    for (int i = 1; i <= len; i++) {
        if (tmp_1[i]) {
            pos_2[cnt] = tmp_1[i];
            cnt++;
        }
    } //拼接成完整的染色体
    cnt = ran + 1;
    for (int i = 1; i <= len; i++) {
        if (tmp_2[i]) {
            pos_1[cnt] = tmp_2[i];
            cnt++;
        }
    }
    for (int i = 1; i <= len; i++) {
        s[tot + 1][i] = pos_2[i];
        s[tot + 2][i] = pos_1[i];
    } //将新产生的染色体加入到种群中
    tot += 2;
}

void mutate(int s[][MAXN], int len, int pos, int &tot) {
    srand((int)time(0) + rand());
    for (int i = 1; i <= len; i++)
        tmp_1[i] = s[pos][i];
    int ran1 = rand() % len + 1;
    int ran2 = rand() % len + 1;
    while (ran1 == ran2)
        ran2 = rand() % len + 1;
    if (ran1 > ran2) {
        int t = ran1;
        ran1  = ran2;
        ran2  = t;
    } //选取中间的某一段，倒置过来
    memcpy(tmp_2, tmp_1, sizeof(tmp_1));
    for (int i = ran1; i <= ran2; i++) {
        tmp_2[i] = tmp_1[ran1 + ran2 - i];
    }
    memcpy(s[tot + 1], tmp_2, sizeof(tmp_2));
    tot++;
}

void man(int n, int s[][MAXN], int len, int num, int total[], double suit[], double pc0, double pcst, double pm0, double pmst, double avg_suit) {
    //avg_suit为初始种群的平均适应度值
    srand((int)time(0) + rand());
    //printa(s,num,len);
    double suit_avg = 0;
    for (int i = 1; i <= num; i++)
        suit_avg += suit[i];
    suit_avg /= num;
    //suit_avg为当前种群的平均适应度值
    double pc = pc0 - (suit_avg - avg_suit) * pcst / avg_suit;
    int pair  = (int)(num * pc); //有几对染色体发生交叉互换
    //int pair=10;
    //printf("%.4f\n",suit_avg);
    //printf("%.4f %.4f %d\n",pc0,pc,pair);
    int tot = num;
    for (int i = 1; i <= pair; i++) {
        int ran1 = rand() % num + 1;
        int ran2 = rand() % num + 1;
        while (ran1 == ran2)
            ran2 = rand() % num + 1;
        crossover(s, len, ran1, ran2, tot); //交叉互换第ran1和第ran2条染色体
    }

    for (int i = num + 1; i <= tot; i++) {
        //cal_time(p,a,n,s,len,i);
        cal_suit(n, s, len, i, total);
    }

    //printa(total,tot);
    //printa(suit,tot);
    //printa(s,tot,len);
    memset(prob, 0, sizeof(prob));

    for (int i = 1; i <= tot; i++) {
        if (suit[i] <= suit_avg)
            prob[i] = pm0;
        else
            prob[i] = pm0 - (suit[i] - suit_avg) * pmst / suit_avg;
    } //计算某条染色体发生变异的概率

    //printa(prob,tot);
    for (int i = 1; i <= tot; i++)
        prob[i] += prob[i - 1];
    //printa(prob,tot);

    int tmp_tot = tot;
    for (int i = 1; i <= num; i++) {
        double ran = rand() * 1.0 / RAND_MAX;
        for (int j = 1; j <= tmp_tot; j++) {
            if (ran >= prob[j - 1] && ran <= prob[j]) {
                //printf("%.4f %.4f %.4f\n",ran,prob[j-1],prob[j]);
                mutate(s, len, j, tot); //对第j条染色体进行变异操作
                break;
            }
        }
    }
    //printf("%d\n",tot-tmp_tot);
    //printa(s,tot,len);
    for (int i = tmp_tot + 1; i <= tot; i++) {
        //cal_time(p,a,n,s,len,i);
        cal_suit(n, s, len, i, total);
    }

    duplicate(n, s, len, tot, num, total); //从tot条染色体中选出num条作为下一代
    //printa(s,num,len);
}

void evaluate(int s[][MAXN], int num, int len, int total[], double suit[]) {
    //printa(total,num);
    //printa(suit,num);
    double avg = 0;
    for (int i = 1; i <= num; i++)
        avg += total[i];
    avg = avg * 1.0 / num;
    printf("%.4f\n", avg);
    /*
    for(int i=1;i<=num;i++) tmp11[i]=total[i];
    for(int i=1;i<=num;i++) tmp22[i]=suit[i];
    sort(tmp11+1,tmp11+num+1);
    sort(tmp22+1,tmp22+num+1);
    printa(tmp11,num);
    printa(tmp22,num);
    */
}

int main() {
    FILE *fp = fopen("in", "r");
    int n, len = 0, num;
    fscanf(fp, "%d", &n); //工件的个数
    for (int i = 1; i <= n; i++) {
        fscanf(fp, "%d", &a[i]);
        len += a[i];
    } //a[i]为每个工件的工序数，len为总的工序数目
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= a[i]; j++) {
            int _m, _t;
            fscanf(fp, "%d%d", &_m, &_t);
            p[i][j].m = _m; //第i个工件的第j道工序所用的机器编号
            p[i][j].t = _t; //第i个工件的第j道工序所用的时间
        }
    }
    fscanf(fp, "%d", &num);      //初始种群规模
    double pc0, pcst, pm0, pmst; //交叉概率初始值，交叉概率成长值，变异概率初始值，变异概率成长值
    fscanf(fp, "%lf%lf%lf%lf", &pc0, &pcst, &pm0, &pmst);
    //printf("%.4f %.4f %.4f %.4f\n",pc0,pcst,pm0,pmst);
    produce(a, n, len, num); //产生num条染色体，存储在数组s中
    //for(int i=1;i<=len;i++) scanf("%d",&s[i]);
    memset(total, 0, sizeof(total));
    for (int i = 1; i <= num; i++) {
        //cal_time(p,a,n,s,len,i);
        cal_suit(n, s, len, i, total); //计算每一条染色体的适应值函数，存在total数组里
    }
    
    //duplicate(n,s,len,num,num,total);
    /*for(int i=1;i<=num;i++) printf("%.6f ",suit[i]);
    printf("\n");
    for(int i=1;i<=num;i++) printf("%.6f ",prob[i]-prob[i-1]);
    printf("\n");
    */
    //crossover(s,len,3,5,tot);
    //mutate(s,len,4,tot);
    /*
    for(int i=1;i<=tot;i++) {
        for(int j=1;j<=len;j++) {
            printf("%d ",s[i][j]);
        }
        printf("\n");
    }
    */

    double avg_suit = 0;
    for (int i = 1; i <= num; i++)
        avg_suit += suit[i];
    avg_suit /= num;

    int T = 30, cnt = 1;
    while (T--) {
        printf("Case#%d\n", cnt++);
        //printf("Current Complete time\n");
        printa(total, num);
        //printa(suit,num);
        man(n, s, len, num, total, suit, pc0, pcst, pm0, pmst, avg_suit); //对整个种群执行进化操作
        //evaluate(s,num,len,total,suit);
        double avg = 0;
        for (int i = 1; i <= num; i++)
            avg += total[i];
        avg = avg * 1.0 / num;

        if (cnt == 31)
            printf("\nFinal Result:%.4f\n", avg); //输出结果，是所有染色体的适应度值的平均值
    }                                             //printf("%.4f %.4f %.4f %.4f\n",pc0,pcst,pm0,pmst);

    /*
    int T=10;
    while(T--) {

    }
    */
    fclose(fp);
}
