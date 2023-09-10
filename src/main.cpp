//
//  main.cpp
//
//  Created by 史家箫 on 2022/8/10.
//


#include <iostream>
#include <cctype>
#include <vector>
#include <ctime>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

enum Operation { up = 1, down = 2, left = 3, right = 4 };

const int N = 12;
// const int perSec = 1000000;

typedef struct Node {
    int x;
    int y;
    int steps;
    int operation;
} Node;

typedef struct Sjx {
    int x;
    int y;
    int steps;
    int scores;
    int repeats;
    int time;
    // int circles;
    int operation;
} Sjx;

typedef struct Game {
    int wholeTime;
    int wholeScore;
    vector <vector <Sjx>> wholeAnswers;
} Game;

typedef struct Map {
    int map[N][N];
    int book[N][N];
    int startx, starty, endx, endy;
} Map;

// Searching solutions, no circles are allowed:
vector <Node> stack;
vector <vector <Node>> answer;


// Initialize our map:
void getMap(Map& mapCurrent, int levelCurrent);
// Find all answers:
void findAnswer(Map& mapCurrent);
// DFS:
void DFS(Map& mapCurrent, int x, int y, int steps);
// Print answers:
void printAnswer();

// Print press:
void press();
// Print after 2 secs:
void wait();
// Print xxm's infomation:
void printInfo(const Game& game, int levelCurrent, bool start);
// Whether you can go this way:
bool judge(const Map& mapCurrent, int x, int y);
// Get Xxm's choice of operation:
Operation getOperation(Map& mapCurrent, Game& game, Sjx& current, int already);
// Create and push our next node into stack(after judging whether illegal):
int getNextPosition(const Sjx& previous);
// Print current situation:
void printCurrentSituation(const Map& mapCurrent, const Sjx& current, bool finished);
// Print congratulations:
void printCongratulation(int levelCurrent);


int main() {
    
    Map mapCurrent;
    
    const int levelAll = 3;
    int levelCurrent = 1;
    
    // interactions:
    printf("\n欢迎来到sjx的小游戏工坊！\n");
    press();
    printf("我们为客户定制了专属游戏，为了您更好的游戏体验，我可以问下您的名字吗？\n");
    string name;
    {
        int flag = 0;
        do {
            if (flag)
                printf("啊，实在抱歉，这里没有您的名字，sjx只为cys设计游戏 :(\n");
            wait();
            printf("您的名字（拼音或缩写）：");
            cin >> name;
            flag++;
        } while (name != "Cys" && name != "cys" && name != "Chengyusi" && name != "chengyusi");
    }
    printf("哈哈！我就知道你是cys, 欢迎!\n\n\n");
    wait();
    
    // Introduction:
    printf("介绍会慢慢跳出来哦\n\n");
    wait();
    printf("\n[简介]\n");
    wait();
    printf("是这样的，她马上要和男朋友去长白山玩啦。\n");
    wait();
    printf("可是呢，cys有一个小问题，她并不知道去长白山的路（doge）。\n");
    wait();
    printf("现在，我们要一起来帮助她先找到男朋友的位置，然后去取车，再去长白山玩！\n");
    wait();
    printf("这里有三幅地图，1代表不可走，0代表可以走，我们用wasd来操控上下左右！\n");
    wait();
    printf("你准备好了嘛！让我们来一起帮助她吧！\n\n");
    press();
    
    Game game = {0, 100};
    // Havn't win the game:
    while (levelCurrent <= levelAll) {
        
        // Xxm's answer:
        vector <Sjx> currentAnswer;
        getMap(mapCurrent, levelCurrent);
        findAnswer(mapCurrent);
        
        bool fail = false;
        
        
        // Initialize:
        int x = mapCurrent.startx, y = mapCurrent.starty, steps = 1;
        Sjx current {x, y, steps, 0, 0};
        mapCurrent.book[x][y] = 1;
        
        // Start game:
        // press();
        time_t start, cur, already, finish;
        printInfo(game, levelCurrent, true);
        time(&start);
        while ( !(current.x == mapCurrent.endx && current.y == mapCurrent.endy) ) {
            // Print situation:
            printCurrentSituation(mapCurrent, current, false);
            
            // Interaction:
            int flag = 0;
            do {
                if (flag) {
                    // wait();
                    printf("\n这会撞墙的！扣10分咯！请再选择一次吧，记得谨慎呀！\n\n");
                    // wait();
                    current.scores -= 10;
                }
                time(&cur);
                already = cur - start;
                current.operation = getOperation(mapCurrent, game, current, int(already));
                flag++;
                if (game.wholeScore + current.scores < 0) {
                    fail = true;
                    printf("啊，很可惜，分数低于零了，让我们再来一次吧！\n");
                    break;
                }
            } while( !(judge(mapCurrent, getNextPosition(current) / N, getNextPosition(current) % N)) );
            
            if (fail) break;
            
            int nextx = getNextPosition(current) / N, nexty = getNextPosition(current) % N;
            if (mapCurrent.book[nextx][nexty] == 0) {
                mapCurrent.book[nextx][nexty] = 1;
                currentAnswer.push_back(current);
            }
            else {
                current.repeats++;
                // wait();
                printf("\n竟然兜兜转转回到了走过的路！扣10分咯！请再选择一次吧，记得谨慎呀！\n\n");\
                // wait();
                current.scores -= 10;
                currentAnswer.push_back(current);
            }
            
            // Update current:
            current.x = nextx;
            current.y = nexty;
            current.steps++;
            
            if (game.wholeScore + current.scores < 0) {
                fail = true;
                printf("啊，很可惜，分数低于零了，让我们再来一次吧！\n");
                break;
            }
        }
        
        if (fail) {
            levelCurrent = 1;
            game.wholeScore = 100;
            game.wholeTime = 0;
            continue;
        }
        // Win this level:
        time(&finish);
        current.time = int(finish - start);
        
        switch (levelCurrent) {
            case 1:
                printf("\n恭喜你成功通过第%d关!cys已找到男朋友的位置！这是你在这关中交出的完美“答卷”！\n\n", levelCurrent);
                break;
            case 2:
                printf("\n恭喜你成功通过第%d关!cys和男朋友已经找到车啦！这是你在这关中交出的完美“答卷”！\n\n", levelCurrent);
                break;
            case 3:
                printf("\n恭喜你成功通过第%d关!cys和男朋友到长白山啦！这是你在这关中交出的完美“答卷”！\n\n", levelCurrent);
                break;
        }
        
        wait();
        if (current.time >= 120) {
            printf("在这关中，因用时超过2分钟而扣20分！\n");
            current.scores -= 20;
        }
        wait();
        current.scores += 100;
        currentAnswer.push_back(current);
        
        printf("你在这关中总共探索了这些位置，我把他们用'·'做了标记：\n");
        wait();
        printCurrentSituation(mapCurrent, current, true);
        printf("你在这关中总共重复探索了相同的位置%d次。\n", current.repeats);
        wait();
        
        // printf("In this level, you have earned %d scores.\n", current.scores);
        
        game.wholeAnswers.push_back(currentAnswer);
        game.wholeScore += current.scores;
        game.wholeTime += current.time;
        printInfo(game, levelCurrent, false);
        
        
        
        
        if (levelCurrent == levelAll && game.wholeScore < 300) {
            levelCurrent = 1;
            printf("啊，很可惜这三关我们只得到了%d分，让我们再来一次吧！\n", game.wholeScore);
            game.wholeScore = 100;
            game.wholeTime = 0;
            game.wholeAnswers.clear();
            press();
            continue;
        }
        
        if (levelCurrent == levelAll)
            wait();
        else press();
        levelCurrent++;
    }
    
    printf("\n\n恭喜你呀！已经成功通过了全部关卡！\n\n\n\n\n");
    wait();
    printf("哦，等一下，sjx同学特意嘱咐我，他好像想对你说些什么：\n\n\n");
    wait();
    printf("我\n");
    wait();
    printf("是\n");
    wait();
    printf("sjx\n");
    wait();
    printf("我\n");
    wait();
    printf("tm\n");
    wait();
    printf("爱死你啦！\n");
    wait();
    printf("\n\n\nThe end.\n\n\n\n");
    return 0;
}

void getMap(Map& mapCurrent, int levelCurrent) {
    
    int map1[N][N] = {
        {1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1},
        {0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}
    };
    
    int map2[N][N] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0},
        {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0}
    };
    
    int map3[N][N] = {
        {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
        {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}
    };
    
    if (levelCurrent == 1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mapCurrent.map[i][j] = map1[i][j];
                mapCurrent.book[i][j] = 0;
            }
        }
        mapCurrent.startx = 0;
        mapCurrent.starty = 5;
        mapCurrent.endx = 11;
        mapCurrent.endy = 5;
    }
    
    else if (levelCurrent == 2) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mapCurrent.map[i][j] = map2[i][j];
                mapCurrent.book[i][j] = 0;
            }
        }
        mapCurrent.startx = 0;
        mapCurrent.starty = 0;
        mapCurrent.endx = 0;
        mapCurrent.endy = 11;
    }
    
    else if (levelCurrent == 3) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mapCurrent.map[i][j] = map3[i][j];
                mapCurrent.book[i][j] = 0;
            }
        }
        mapCurrent.startx = 5;
        mapCurrent.starty = 8;
        mapCurrent.endx = 11;
        mapCurrent.endy = 11;
    }
    
    
}

void findAnswer(Map& mapCurrent) {
    stack.clear();
    answer.clear();
    int x = mapCurrent.startx, y = mapCurrent.starty, steps = 1;
    DFS(mapCurrent, x, y, steps);
}

void DFS(Map& mapCurrent, int x, int y, int steps) {
    
    Node current = {x, y, steps};
    
    // Reach the end:
    if (x == mapCurrent.endx && y == mapCurrent.endy) {
        stack.push_back(current);
        answer.push_back(stack);
        return;
    }
        
    
    // Havn't reach the end:
    mapCurrent.book[x][y] = 1;
    
    // Get operation:
    for (current.operation = up; current.operation <= right; current.operation++) {
        int tempx = 0, tempy = 0;
        switch (current.operation) {
            case up:    tempx--; break;
            case down:  tempx++; break;
            case left:  tempy--; break;
            case right: tempy++; break;
        }
        if (judge(mapCurrent, x+tempx, y+tempy) && mapCurrent.book[x+tempx][y+tempy] == 0) {
            stack.push_back(current);
            DFS(mapCurrent, x+tempx, y+tempy, ++steps);
            stack.pop_back();
            
            steps--;
        }
    }
    mapCurrent.book[x][y] = 0;
    return;
}

bool judge(const Map& mapCurrent, int x, int y) {
    return (x < 0 || y < 0 || x >= N || y >= N || mapCurrent.map[x][y] == 1) ? false : true;
}

void printAnswer() {
    for (int i = 0; i < answer.size(); i++) {
        printf("Solution%2d:\n", i+1);
        for (int j = 0; j < answer[i].size() - 1; j++) {
            printf("steps%3d: from (%2d, %2d) ",answer[i][j].steps, answer[i][j].x, answer[i][j].y);
            switch (answer[i][j].operation) {
                case up:    printf("move up    to "); break;
                case down:  printf("move down  to "); break;
                case left:  printf("move left  to "); break;
                case right: printf("move right to "); break;
            }
            printf("(%2d, %2d).\n", answer[i][j+1].x, answer[i][j+1].y);
        }
        putchar('\n');
    }
}

void printCurrentSituation(const Map& mapCurrent, const Sjx& current, bool finished) {
    
    putchar('\n');
    if (finished) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (mapCurrent.book[i][j] == 1)
                    printf("· ");
                else
                    printf("%d ", mapCurrent.map[i][j]);
            }
            putchar('\n');
        }
        putchar('\n');
        return;
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == current.x && j == current.y)
                printf("X ");
            else if (i == mapCurrent.endx && j == mapCurrent.endy)
                printf("E ");
            else if (mapCurrent.book[i][j] == 1)
                printf("· ");
            else
                printf("%d ", mapCurrent.map[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

Operation getOperation(Map& mapCurrent, Game& game, Sjx& current, int already) {
    char answer;
    time_t start, cur;
    time(&start);
    do {
        printf("请做出你的选择(用wasd代表上下左右，h查看本关答案，i查看当前信息)：");
        scanf(" %c", &answer);
        fflush(stdin);
        answer = toupper(answer);
        switch (answer) {
            case 'W': return up;
            case 'A': return left;
            case 'S': return down;
            case 'D': return right;
            case 'H': printAnswer();
                current.scores -= 15;
                wait();
                if (game.wholeScore + current.scores < 0)
                    return up;
                // printf("%d", CLK_TCK);
                printCurrentSituation(mapCurrent, current, false);
                break;
            case 'I': printf("本关之前总得分：%d\n", game.wholeScore);
                printf("本关目前得分：%d\n", current.scores);
                time(&cur);
                printf("本关目前耗时：%lu\n", already + cur - start);
                wait();
                break;
            // default: continue;
        }
    } while (true);
}

int getNextPosition(const Sjx& previous) {
    int tempx = 0, tempy = 0;
    switch (previous.operation) {
        case up:    tempx--; break;
        case down:  tempx++; break;
        case left:  tempy--; break;
        case right: tempy++; break;
    }
    return (previous.x + tempx) * N + (previous.y + tempy);
}

void printInfo(const Game& game, int levelCurrent, bool start) {
    
    if (start) {
        switch (levelCurrent) {
            case 1:
                printf("\n欢迎来到关卡%d：cys要去找男朋友！\n\n", levelCurrent);
                break;
            case 2:
                printf("\n欢迎来到关卡%d：cys要和男朋友去找车！\n", levelCurrent);
                break;
            case 3:
                printf("\n欢迎来到关卡%d：cys要和npy去长白山玩！\n", levelCurrent);
                break;
        }
        wait();
        if (levelCurrent != 1) {
            printf("当前总分：%d\n", game.wholeScore);
            printf("总共耗时：%ds\n", game.wholeTime);
            printf("\n来看地图！\n");
            press();
        }
        else {
            printf("在第一关开始的时候，我会给你100分！\n");
            wait();
            printf("记得不要撞墙或者走重复的路哦！这样会扣10分的！\n");
            wait();
            printf("你走过的路我会用'·'帮你标记的！\n");
            wait();
            printf("通过每一关后，我会给你100分！\n");
            wait();
            printf("如果你在一关中花费了超过2分钟的话，会扣20分哦！\n");
            wait();
            printf("如果你的分数低于0了，我们只能再来一次啦！\n");
            wait();
            printf("如果通过第三关后你的分数高于300，则cys和男朋友就可以顺利去长白山玩啦！\n");
            wait();
            printf("哦对了，如果你在游戏过程中的任何时候想要查看本关答案的话，输入'h'（help）就好啦，这将花费15分。\n");
            wait();
            printf("还有，如果你在游戏过程中的任何时候想要查看当前自己的“账户”情况，输入'i'（information）就好啦。\n");
            wait();
            printf("最后，在地图中，X代表当前位置，E代表终点位置。\n");
            wait();
            printf("准备好了吗？那就让我们开始吧！\n\n来看地图！\n");
            press();
        }
    }
    
    else {
        printf("在第%d个关卡中, 你总共得到了%d分, 耗时%ds\n", levelCurrent, (game.wholeAnswers[levelCurrent-1].end()-1)->scores, (game.wholeAnswers[levelCurrent-1].end()-1)->time);
        wait();
        printf("在完成关卡%d后：\n", levelCurrent);
        wait();
        printf("当前总分：%d\n", game.wholeScore);
        printf("总共耗时：%ds\n\n\n\n", game.wholeTime);
        // wait();
    }
}

void press() {
    fflush(stdin);
    printf("\nPress any key to continue...\n");
    getchar();
    fflush(stdin);
}

void wait() {
    clock_t start = clock();
    while ( ((clock() - start) / CLK_TCK) < 12500);
}
