#include "gba1.h"
#include "8x8ascii.h"
// 寿司用　色宣言
#define BLK	BGR(0x00,0x00,0x00)	// 黒
#define WHI	BGR(0x1F,0x1F,0x1F)	// 白
#define RED	BGR(0x1F,0x00,0x00)	// 赤
#define YEL	BGR(0x1F,0x1F,0x00) // 黄
#define ORG BGR(0x1F,0x14,0x00) // オレンジ
#define BEI BGR(0x1F,0x1A,0x19)	// ベージュ
#define GRN BGR(0x13,0x1C,0x13)	// 緑
#define GRY BGR(0x13,0x13,0x13)	// グレー

// その他の色宣言
#define DARKORANGE	BGR(0x1F, 0x11, 0x00)	// ダークオレンジ
#define WHITE	BGR(0x1F, 0x1F, 0x1F)		// 白
#define BLACK	BGR(0x00, 0x00, 0x00)		// 黒
#define GREEN	BGR(0x13, 0x1C, 0x13)		// 薄い緑
#define GREY	BGR(0x13, 0x13, 0x13)		// グレー


/* ゲームの調整はここで */
#define TIME_LIMIT	30	// 時間制限
#define SPEED	5		// 寿司の進む速さ


/*	メモ　
	レベルに合わせてコマンドの長さ調節
	コマンドの長さに合わせて金額調整
	コマンドの長さはランダムで増減する
*/


// プロトタイプ宣言
void paint_under(hword, hword);
hword prepare(hword);
hword game(hword);
hword result(hword, hword, hword);
void default_display();
void display_time(hword, hword);
void display_sushi(hword);
void draw_point(hword, hword, hword);
void locate(hword, hword);
void print_ascii(unsigned char, hword, hword);
void prints(unsigned char*, hword, hword);
void printn(hword i, hword, hword);
hword mod(hword, hword);
hword div(hword, hword);
void make_command(hword, hword, hword, hword, hword);

// 寿司のドット絵
hword tuna[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,RED,RED,RED,BLK,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,BLK,RED,RED,RED,BLK,BLK,RED,RED,RED,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,RED,RED,RED,BLK,BLK,RED,RED,RED,RED,RED,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,RED,RED,BLK,RED,RED,RED,RED,RED,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,BLK,BLK,RED,RED,RED,BLK,BLK,WHI,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,WHI,WHI,BLK,BLK,BLK,WHI,WHI,WHI,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,BLK,WHI,WHI,WHI,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

hword ika[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,WHI,WHI,WHI,BLK,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,BLK,WHI,WHI,WHI,BLK,BLK,WHI,WHI,WHI,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,WHI,WHI,WHI,BLK,BLK,WHI,WHI,WHI,WHI,WHI,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,WHI,WHI,BLK,WHI,WHI,WHI,WHI,WHI,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,BLK,BLK,WHI,WHI,WHI,BLK,BLK,WHI,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,WHI,WHI,BLK,BLK,BLK,WHI,WHI,WHI,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,BLK,WHI,WHI,WHI,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

hword samon[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,ORG,ORG,ORG,BLK,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,BLK,ORG,ORG,ORG,BLK,BLK,ORG,ORG,ORG,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,ORG,ORG,ORG,BLK,BLK,ORG,ORG,ORG,ORG,ORG,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,ORG,ORG,BLK,ORG,ORG,ORG,ORG,ORG,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,BLK,BLK,ORG,ORG,ORG,BLK,BLK,WHI,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,WHI,WHI,BLK,BLK,BLK,WHI,WHI,WHI,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,BLK,WHI,WHI,WHI,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

hword shrimp[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,RED,BLK,GRN,BLK,BLK,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,WHI,RED,RED,BLK,BLK,RED,BLK,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,BLK,WHI,ORG,ORG,BLK,BLK,WHI,RED,RED,BLK,GRN,GRN},
	{GRN,GRN,GRN,BLK,WHI,ORG,ORG,BLK,BLK,WHI,ORG,ORG,WHI,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,ORG,WHI,BLK,WHI,ORG,ORG,WHI,ORG,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,BLK,BLK,ORG,WHI,ORG,BLK,BLK,WHI,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,WHI,WHI,BLK,BLK,BLK,WHI,WHI,WHI,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,BLK,WHI,WHI,WHI,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

hword kappa[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,GRN,BLK,WHI,GRN,WHI,BLK,BLK,BLK,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,BLK,BLK,BLK,WHI,BLK,WHI,GRN,WHI,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,WHI,GRN,WHI,BLK,BLK,BLK,WHI,WHI,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,WHI,WHI,BLK,WHI,GRN,WHI,BLK,GRY,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,GRY,GRY,BLK,WHI,WHI,WHI,BLK,GRY,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,GRY,GRY,BLK,GRY,GRY,GRY,BLK,GRY,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,GRY,GRY,BLK,GRY,GRY,GRY,BLK,GRY,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,BLK,GRY,GRY,GRY,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

hword ikura[13][17]={
	{GRN,GRN,GRN,GRN,GRN,GRN,GRN,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,GRN,BLK,BLK,WHI,ORG,BLK,BLK,GRN,GRN,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,WHI,ORG,BLK,BLK,WHI,ORG,BLK,BLK,GRN,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,ORG,BLK,WHI,ORG,ORG,ORG,WHI,ORG,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,BLK,BLK,ORG,ORG,WHI,ORG,ORG,ORG,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,GRN,BLK,GRY,BLK,BLK,BLK,ORG,ORG,BLK,BLK,BLK,GRN,GRN,GRN},
	{GRN,GRN,GRN,BLK,BLK,GRY,BLK,GRY,GRY,BLK,BLK,GRY,GRY,BLK,BLK,GRN,GRN},
	{BEI,BEI,BLK,YEL,BLK,GRY,BLK,GRY,GRY,GRY,GRY,GRY,GRY,BLK,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,BLK,BLK,GRY,GRY,GRY,GRY,BLK,BLK,YEL,YEL,BLK,BEI},
	{BEI,BEI,BLK,YEL,YEL,YEL,YEL,BLK,BLK,BLK,BLK,YEL,YEL,YEL,YEL,BLK,BEI},
	{BEI,BEI,BEI,BLK,BLK,YEL,YEL,YEL,YEL,YEL,YEL,YEL,YEL,BLK,BLK,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BLK,BEI,BEI,BEI,BEI},
	{BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI,BEI}
};

// コマンド表示用変数
unsigned char command_row[10];		// 表示するコマンドを格納する
unsigned char command_number[10];	// 表示するコマンドの、commands_asciiの要素番号を格納する
unsigned char commands_ascii[8]={65, 66, 24, 25, 26, 27, 82, 76};	// 各コマンドのasciiコードを格納  {A, B, ↑, ↓, →, ←, R, L}


// グローバル変数の宣言
hword* key;	// キー入力レジスタ
hword key2;	// キー入力レジスタの内容保持
int i, j, k;	// for文カウント用

point sushi_left;	// 寿司の左端の座標
point sushi_left_ini;	// sushi_leftの初期値

point p_display;	// 文字の表示位置のディスプレイ座標

// メイン
int main(void) {

	// 描画モードの設定
	*((hword *)IOBASE) = 0x0F03;

	// キー入力レジスタの指定
	key = (hword*)0x04000130;

	hword again;	// 1: try again / 0: not
	hword level = 1;
	

	while(1){	// 再挑戦用のループ

		// デフォルト画面の表示
		default_display();

		// 説明画面
		locate(2,2);
		prints("Type the keys", BLK, GRN);

		locate(2,3);
		prints("shown on the screen", BLK, GRN);

		locate(2,4);
		prints("from left to right", BLK, GRN);

		locate(2,5);
		prints("before sushi go away!", BLK, GRN);

		locate(2,7);
		prints("Press SELECT to exit", BLK, GRN);


		locate(2, 9);
		prints("Press A to Game Start!!", BLK, GRN);


		// キーが押されるのを待つ
		while(1){
			key2 = ~*key&0x03FF;	// キー入力レジスタの値取得, 不定ビットの対応

			if(key2==KEY_A){	// Aが押され、離されたらゲームスタート
				while((~*key&0x03FF)==KEY_A){
				}
				break;
			}else if(key2==KEY_RIGHT){
				if(level!=3){
					level++;
				}
				while((~*key&0x03FF)==KEY_RIGHT){
				}
			}else if(key2==KEY_LEFT){
				if(level!=1){
					level--;
				}
				while((~*key&0x03FF)==KEY_LEFT){
				}
			}

			locate(20, 11);
			prints("level ", BLK, GRN);
			locate(27, 11);
			printn(level, BLK, GRN);			
		}

		// ゲームの開始
		again = prepare(level);	// 返り値が1ならもう一度最初から

		if(again==1){
			continue;
		}else{
			break;
		}

	}

	// デフォルト画面
	default_display();

	locate(10, 8);
	prints("Thank you!!", BLK, GRN);

	while (1);	// 無限ループで停止
	return 0;

}

hword prepare(hword level){

	// ゲーム開始前の画面設定
	// 画面の枠をオレンジにする
	paint_under(DARKORANGE, 0);

	// 中を白くする
	for(i=20; i<160-5; i++){
		for(j=5; j<240-5; j++){
			draw_point(j, i, WHITE);
		}
	}

	// ゲージ
	for(i=11; i<15; i++){
		for(j=118; j<230; j++){
			draw_point(j, i, WHITE);
		}
	}

	// 背景緑
	for(i=20; i<20+4*7; i++){
		for(j=5; j<240-5; j++){
			draw_point(j, i, GREEN);
		}
	}


	// コマンド入力画面
	for(i=78; i<122; i++){
		for(j=28; j<202; j++){
			draw_point(j, i, BLK);
		}
	}
	for(i=80; i<120; i++){
		for(j=30; j<200; j++){
			draw_point(j, i, GREY);
		}
	}

	// メーター
	locate(14, 0);
	prints("    +1   +2   +3", BLACK, DARKORANGE);

	for(i=9; i<11; i++){
		for(j=150; j<154; j++){
			draw_point(j, i, BLK);
		}
	}
	for(i=9; i<11; i++){
		for(j=190; j<194; j++){
			draw_point(j, i, BLK);
		}
	}
	for(i=9; i<11; i++){
		for(j=226; j<230; j++){
			draw_point(j, i, BLK);
		}
	}


	// タイマー用設定
	locate(2,1);
	prints("Time = ", BLACK, ORG);

	//レーンの表示
	for(i=20+7*4; i<20+13*4; i++){
		for(j=5; j<240-5; j++){
			draw_point(j, i, BEI);
		}
	}

	return game(level);
}


hword game(hword level){

	// タイマの値取得用
	hword time, time2, time3, time4;
	
	// 制限時間
	hword time_limit;
	time_limit = TIME_LIMIT;

	// 座標設定用
	hword x, y;

	point command_teacher;	// 手本の表示用
	point command;			// 入力された文字の表示用

	x = 5;
	y = 10;

	sushi_left_ini.x = 5;
	sushi_left_ini.y = 20;
 

	sushi_left.x = sushi_left_ini.x;
	sushi_left.y = sushi_left_ini.y;
	// 寿司を4倍にして表示
	display_sushi(1);


	y = 10;

	hword step = 2;		// 1度のすし移動で何ドット移動するか
	hword count = 0;	// 寿司の移動回数カウント
	hword speed = 0;		// 何フレームに一回移動するか

	key = (hword*)0x04000130;	// キー入力レジスタの設定

	hword key_continue=0;	// 長押しの検出
    hword command_count = 0;	// 何個目の文字を入力しているか0~

    hword show = 0;	// 1になったら新しいコマンドを出現させる

    hword command_num = 0;	// 配列の何番目のコマンドを出現させるか
	hword type = 1;	// 寿司のタイプ

	hword sushi_counter=0;	// 何皿寿司をゲットしたか

	int finish=0;	// コマンドを10個入力し終わったら1に
	int mistake=0; 	// 打ち間違いの検知
	int plus_time=0;	// タイム追加
	hword continuing=0;	// 正しいキー入力の連続回数
	hword input=0;		// 入力の検知

	hword type_count=0;	// 正しく入力したキーの数
	hword mistype_count=0;	// 誤って入力したキーの数
	hword num;	// 入力されたキーのasciiコード
	hword end = 0; // 1になったらリザルトへ

	hword command_length = level*3;	// 最大10

	// コマンドが入力される座標
	command.x = 5;
	command.y = 13;

	// ゲームスタート直後のカウントダウン
	for(i=0; i<=800; i++){
		locate(command.x, command.y);
		prints("3", BLK, GREY);
	}
	for(i=0; i<=800; i++){
		locate(command.x, command.y);
		prints("2", BLK, GREY);
	}
	for(i=0; i<=800; i++){
		locate(command.x, command.y);
		prints("1", BLK, GREY);
	}
	// カウントダウンが終わったら塗りつぶす
	locate(command.x, command.y);
	prints("  ", GREY, GREY);

	/* タイマカウンタ設定レジスタ */
	*((unsigned short *)0x04000100) = 0xFF00;	// タイマ0 カウンタ設定(元クロック1/2^24秒 約60n秒)
	*((unsigned short *)0x04000104) = 0xFF00;	// タイマ1 カウンタ設定(元クロック1/2^16秒 約15.4u秒)
	*((unsigned short *)0x04000108) = 0xFF00;	// タイマ2 カウンタ設定(元クロック1/2^8秒  約3.93m秒)
	*((unsigned short *)0x0400010C) = 0x0000;	// タイマ3 カウンタ設定(元クロック1/2^8秒  約1秒)

	/* タイマ制御レジスタ */	
	*((unsigned short *)0x04000102) = 0x0080;	// タイマ0 制御設定（タイマON，割込みOFF，カスケードOFF，プリスケーラなし）
	*((unsigned short *)0x04000106) = 0x0084;	// タイマ1 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010A) = 0x0084;	// タイマ2 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010E) = 0x0084;	// タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）



	while(1){
		// ゲーム処理

		// キー処理初期化
		key2 = ~*key & 0x03FF;

		// キーが押され続けている場合は、key_continue=1
		if(key_continue==1 && key2==0){
			key_continue = 0;
		}

		/* タイマ3 カウント値読込み */
		time = 	*((unsigned short *)0x0400010C);

		// タイマ表示
		locate(9,1);
		display_time(time_limit-time, 3);
		if(time_limit-time==0){
			break;
		}
	

		// 寿司の移動
		if(speed==SPEED){
			if(finish==1){	// コマンド入力が時間内に完了したとき

				// 消す
				for(i=0; i<17*4; i++){
					for(j=0; j<(13-6)*4; j++){
						draw_point(sushi_left.x, sushi_left.y++, GRN);
					}
					for(j=0; j<6*4;j++){
						draw_point(sushi_left.x, sushi_left.y++, BEI);
					}
					sushi_left.x++;
					sushi_left.y = sushi_left_ini.y;
				}

				// 寿司の位置を初期化
				sushi_left.x = sushi_left_ini.x;
				// sushi_left.y = sushi_left_ini.y; 前のfor文中で実行されてる
				
				// 寿司のタイプ変更
				type = (type==6) ? 1 : type+1;
				
				count =0;
				finish = 0;

			}else if(count<=80){ 	// 移動
				
				sushi_left.x += 2;
				display_sushi(type);
			
				count++;

			}else if(count==81){	// 端まで来たら消して左に再表示
				x = 5 + (80+1)*step;
				// y = 20;	// これはsushi_leftに変えられない

				for(i=0; i<17*4; i++){
					for(j=0; j<(13-6)*4; j++){
						draw_point(x, sushi_left.y++, GRN);
					}
					for(j=0; j<6*4;j++){
						draw_point(x, sushi_left.y++, BEI);
					}
					x++;
					sushi_left.y = 20;
				}

				// 寿司を4倍にして表示
				sushi_left.x = sushi_left_ini.x;
				type = (type==6) ? 1 : type+1;
				
				count =0;

				// コマンドの更新
				command_num++;
				command_count=0;
				command.x = 5;
				command.y = 13;
				locate(command.x,command.y);
				prints("                    ", GREY, GREY);
				show = 0;

				// メーターを白にする
				continuing = 0;
				for(i=11; i<15; i++){
					for(j=118; j<230; j++){
						draw_point(j, i, WHITE);
					}
				}
					
			}

			speed = 0;
		}else{
			speed++;
		}

		
		
		// 文字

        if(show==0){
            command_teacher.x = 5;
            command_teacher.y = 11;

			time4 = *((unsigned short *)0x04000100);
			time2 =	*((unsigned short *)0x04000104);
			time3 = *((unsigned short *)0x04000108);
			
			make_command(time, time2-0xFF00, time3-0xFF00, time4-0xFF00, count);
			
            for(i=0; i<command_length; i++){
				
                locate(command_teacher.x++, command_teacher.y);
                print_ascii(command_row[i], BLK, GREY);
                command_teacher.x++;
			
            }
            show = 1;
        }       


        if(key_continue==0){	// 長押しでないことの確認
            
            switch(key2){	// キー入力の取得

                case KEY_A:
                    if(command_row[command_count]==65){	// 正しいキーを入力しているか判定
						num = 65;	// 表示するasciiコード(A)
						input = 1;	// フラグを立てる
                    }else{
						mistake=1;
					}                   
                    break;

                case KEY_B:
                    if(command_row[command_count]==66){
                        num = 66;
						input = 1;
                    }
					else{
						mistake=1;
					}
                    break;

                case KEY_UP:
                    if(command_row[command_count]==24){
                        num = 24;
						input = 1;

                    }else{
						mistake=1;
					}
                    break;

                case KEY_DOWN:
                    if(command_row[command_count]==25){
                        num = 25;
						input=1;
                    }else{
						mistake=1;
					}
                    break;

                case KEY_RIGHT:
                    if(command_row[command_count]==26 ){
                        num = 26;
						input =1;
                    }else{
						mistake=1;
					}
					
                    break; 

                case KEY_LEFT:
                    if(command_row[command_count]==27){
                        num = 27;
						input =1;
                    }else{
						mistake=1;
					}
                    break;

				case KEY_R:
                    if(command_row[command_count]==82){
                        num = 82;
						input =1;
                    }else{
						mistake=1;
					}
                    break;

				case KEY_L:
                    if(command_row[command_count]==76){
                        num = 76;
						input =1;
                    }else{
						mistake=1;
					}
                    break;

                case KEY_SELECT:
					end = 1;  
            }
        
        }

		if(input==1){
			locate(command.x++, command.y);
			print_ascii(num, BLK, GREY);
			key_continue = 1;
			command.x++;
			command_count++;
			continuing++;
			type_count++;
			input=0;
			
			if(mistake==0){
				for(i=11; i<15; i++){
					for(j=118; j<118+continuing*4; j++){
						draw_point(j, i, RED);
					}
				}
			}
		}

		// タイム追加処理
		if(continuing==9 && plus_time==0){
			time_limit += 1;
			plus_time = 1;
		}
		if(continuing==19 && plus_time==1){
			time_limit += 2;
			plus_time = 0;
		}
		if(continuing==28 && plus_time==0){
			time_limit += 3;
			continuing = 0;
			for(i=11; i<15; i++){
				for(j=118; j<230; j++){
					draw_point(j, i, WHITE);
				}
			}
		}

		// ミスしたとき
		if(mistake==1){
			key_continue = 1;
			mistype_count++;
			continuing=0;
			mistake=0;
			for(i=11; i<15; i++){
				for(j=118; j<228; j++){
					draw_point(j, i, WHITE);
				}
			}
		}

		// 打ち終わり
        if(command_count==command_length){
            show = 0;
            command_count=0;
            command.x = 5;
            command.y = 13;

			// 消す
            locate(command.x,command.y);
			prints("                    ", GREY, GREY);

			// 教師も
			command_teacher.x = 5;
			command_teacher.y = 11;
			locate(command_teacher.x, command_teacher.y);
			prints("                    ", GREY, GREY);

            command_num++;
			sushi_counter++;
			finish = 1;
        }

		if(end==1){
			break;
		}


		// 皿数の表示
		locate(3, 17);
		prints("sushi counter", BLK, WHI);
		locate(19, 17);
		printn(sushi_counter, BLK, WHI);
		
	}// while終わり


	/* タイマ制御レジスタ タイマオフ*/
	*((unsigned short *)0x04000102) = 0x0000;	// タイマ0 制御設定（タイマOFF，割込みOFF，カスケードOFF，プリスケーラなし）
	*((unsigned short *)0x04000106) = 0x0004;	// タイマ1 制御設定（タイマOFF，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010A) = 0x0004;	// タイマ2 制御設定（タイマOFF，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010E) = 0x0004;	// タイマ3 制御設定（タイマOFF，割込みOFF，カスケードON，プリスケーラなし）

	return result(sushi_counter, type_count, mistype_count);

}

// リザルト
hword result(hword sushi_counter, hword type_count, hword mistype_count){

	default_display();

	locate(5, 2);
	prints("You got ", BLK, GRN);
	locate(14, 2);
	printn(sushi_counter*100, BLK, GRN);
	locate(19, 2);
	prints("yen!", BLK, GRN);

	locate(4, 4);
	prints("Type counter ", BLK, GRN);
	locate(18, 4);
	printn(type_count, BLK, GRN);

	locate(3, 6);
	prints("Mistype counter ", BLK, GRN);
	locate(20,6);
	printn(mistype_count, BLK, GRN);

	locate(3, 9);
	prints("Try again ... press A", BLK, GRN);

	locate(3, 10);
	prints("End game  ... press B", BLK, GRN);

	while(1){
		key2 = ~*key&0x03FF;
		
		switch(key2){
			case KEY_A:
				while((~*key&0x03FF)==KEY_A){

				}
				return 1;
				break;

			case KEY_B:
				while((~*key&0x03FF)==KEY_B){

				}
				return 0;
				break;
		}
	}

}

void default_display(){
	// ゲーム基本画面の設定
	paint_under(GRN, 0);
	paint_under(BEI, 128);	

	// 寿司の表示
	sushi_left.y = 100;

	sushi_left.x = 180;
	display_sushi(6);

	sushi_left.x = 120;
	display_sushi(5);

	sushi_left.x = 60;
	display_sushi(2);

	sushi_left.x = 0;
	display_sushi(1);
}


// 引数 y よりも y座標が大きい部分を塗る
// ex. y=0　→　全画面
void paint_under(hword color, hword y){
	hword* ptr;
	ptr = (hword*)VRAM;
	int i, j;

	for(i=y;i<160;i++){
		for(j=0;j<240; j++){
			draw_point(j,i,color);
		}
	}
}




void display_time(hword val, hword digit) {
	
	
/* 引数を10進数として表示する関数（最大値16ビット）*/

	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i;
	
	/* 入力値valの桁数設定 */
	i = digit-1;

	/* 文字列の最後にNULLコード挿入 */
	buf[i+1] = 0;
	
	/* 最下位桁の文字（10で割った余り）を挿入し，入力値を10で割る */
	for(; i >= 0; i--) {
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}
	
	/* 文字列全体を表示 */
	prints(buf, BLACK, ORG);
	
	return;
}

// 寿司の描画
void display_sushi(hword type){

	hword x, y;
	
	//y = 10;
	x = sushi_left.x;
	y = sushi_left.y;
	int i,j;

	if(type == 1){
		// 寿司を4倍にして表示
		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , tuna[i][j]);
				draw_point(x+1, y, tuna[i][j]);
				draw_point(x+2, y, tuna[i][j]);
				draw_point(x+3, y , tuna[i][j]);
				draw_point(x, y+1 , tuna[i][j]);
				draw_point(x+1, y+1, tuna[i][j]);
				draw_point(x+2, y+1,tuna[i][j]);
				draw_point(x+3, y+1 , tuna[i][j]);
				draw_point(x, y+2, tuna[i][j]);
				draw_point(x+1, y+2, tuna[i][j]);
				draw_point(x+2, y+2, tuna[i][j]);
				draw_point(x+3, y+2 , tuna[i][j]);
				draw_point(x, y+3 , tuna[i][j]);
				draw_point(x+1, y+3, tuna[i][j]);
				draw_point(x+2, y+3, tuna[i][j]);
				draw_point(x+3, y+3 , tuna[i][j]);

				x += 4;
			}
			y+=4;
			x = sushi_left.x;
		}

	}
	else if(type == 4){

		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , samon[i][j]);
				draw_point(x+1, y, samon[i][j]);
				draw_point(x+2, y, samon[i][j]);
				draw_point(x+3, y , samon[i][j]);
				draw_point(x, y+1 , samon[i][j]);
				draw_point(x+1, y+1, samon[i][j]);
				draw_point(x+2, y+1,samon[i][j]);
				draw_point(x+3, y+1 , samon[i][j]);
				draw_point(x, y+2, samon[i][j]);
				draw_point(x+1, y+2, samon[i][j]);
				draw_point(x+2, y+2, samon[i][j]);
				draw_point(x+3, y+2 , samon[i][j]);
				draw_point(x, y+3 , samon[i][j]);
				draw_point(x+1, y+3, samon[i][j]);
				draw_point(x+2, y+3, samon[i][j]);
				draw_point(x+3, y+3 , samon[i][j]);

				x += 4;
				
			}
			y+=4;
			x = sushi_left.x;
		}
	}else if(type==3){
		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , ika[i][j]);
				draw_point(x+1, y, ika[i][j]);
				draw_point(x+2, y, ika[i][j]);
				draw_point(x+3, y , ika[i][j]);
				draw_point(x, y+1 , ika[i][j]);
				draw_point(x+1, y+1, ika[i][j]);
				draw_point(x+2, y+1,ika[i][j]);
				draw_point(x+3, y+1 , ika[i][j]);
				draw_point(x, y+2, ika[i][j]);
				draw_point(x+1, y+2, ika[i][j]);
				draw_point(x+2, y+2, ika[i][j]);
				draw_point(x+3, y+2 , ika[i][j]);
				draw_point(x, y+3 , ika[i][j]);
				draw_point(x+1, y+3, ika[i][j]);
				draw_point(x+2, y+3, ika[i][j]);
				draw_point(x+3, y+3 , ika[i][j]);

				x += 4;
				
			}
			y+=4;
			x = sushi_left.x;
		}
	}
	else if(type==2){
		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , shrimp[i][j]);
				draw_point(x+1, y, shrimp[i][j]);
				draw_point(x+2, y, shrimp[i][j]);
				draw_point(x+3, y , shrimp[i][j]);
				draw_point(x, y+1 , shrimp[i][j]);
				draw_point(x+1, y+1, shrimp[i][j]);
				draw_point(x+2, y+1,shrimp[i][j]);
				draw_point(x+3, y+1 , shrimp[i][j]);
				draw_point(x, y+2, shrimp[i][j]);
				draw_point(x+1, y+2, shrimp[i][j]);
				draw_point(x+2, y+2, shrimp[i][j]);
				draw_point(x+3, y+2 , shrimp[i][j]);
				draw_point(x, y+3 , shrimp[i][j]);
				draw_point(x+1, y+3, shrimp[i][j]);
				draw_point(x+2, y+3, shrimp[i][j]);
				draw_point(x+3, y+3 , shrimp[i][j]);

				x += 4;
				
			}
			y+=4;
			x = sushi_left.x;
		}
	}else if(type==5){
		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , kappa[i][j]);
				draw_point(x+1, y, kappa[i][j]);
				draw_point(x+2, y, kappa[i][j]);
				draw_point(x+3, y , kappa[i][j]);
				draw_point(x, y+1 , kappa[i][j]);
				draw_point(x+1, y+1, kappa[i][j]);
				draw_point(x+2, y+1,kappa[i][j]);
				draw_point(x+3, y+1 , kappa[i][j]);
				draw_point(x, y+2, kappa[i][j]);
				draw_point(x+1, y+2, kappa[i][j]);
				draw_point(x+2, y+2, kappa[i][j]);
				draw_point(x+3, y+2 , kappa[i][j]);
				draw_point(x, y+3 , kappa[i][j]);
				draw_point(x+1, y+3, kappa[i][j]);
				draw_point(x+2, y+3, kappa[i][j]);
				draw_point(x+3, y+3 , kappa[i][j]);

				x += 4;
				
			}
			y+=4;
			x = sushi_left.x;
		}
	}else if(type==6){
		for(i=0; i<13; i++){
			for(j=0; j<17; j++){
				draw_point(x, y , ikura[i][j]);
				draw_point(x+1, y, ikura[i][j]);
				draw_point(x+2, y, ikura[i][j]);
				draw_point(x+3, y , ikura[i][j]);
				draw_point(x, y+1 , ikura[i][j]);
				draw_point(x+1, y+1, ikura[i][j]);
				draw_point(x+2, y+1,ikura[i][j]);
				draw_point(x+3, y+1 , ikura[i][j]);
				draw_point(x, y+2, ikura[i][j]);
				draw_point(x+1, y+2, ikura[i][j]);
				draw_point(x+2, y+2, ikura[i][j]);
				draw_point(x+3, y+2 , ikura[i][j]);
				draw_point(x, y+3 , ikura[i][j]);
				draw_point(x+1, y+3, ikura[i][j]);
				draw_point(x+2, y+3, ikura[i][j]);
				draw_point(x+3, y+3 , ikura[i][j]);

				x += 4;
				
			}
			y+=4;
			x = sushi_left.x;
		}
	}
	
	
}


void locate(hword x, hword y){
    p_display.x = 8*x;
    p_display.y = 8*y;
}

// 文字の描画		引数：asciiコード
void print_ascii(unsigned char ascii_num, hword color, hword bgcolor){
    int i, j;
    unsigned char cbit = 0x80;
    for(i=0; i<8; i++){
        cbit = 0x80;
        for(j=0; j<8; j++){
            if((char8x8[ascii_num][i]&cbit)==cbit){
                draw_point(p_display.x+j, p_display.y+i, color);
            }else{
				draw_point(p_display.x+j, p_display.y+i, bgcolor);
			}
            cbit = cbit>>1;
        }
    }
}

// 文字列の描画		引数：ポインタ
void prints(unsigned char *str, hword color, hword bgcolor){
    
    while(*str!=0x00){
        print_ascii(*str++, color, bgcolor);
        p_display.x += 8;
    }
}

void draw_point(hword x, hword y, hword color){
    hword *ptr;
    ptr = (hword*)VRAM;
    ptr += x + 240*y;

    *ptr = color;
}

// 引数を10進数として表示する
unsigned char tmp[8]={0,0,0,0,0,0,0,0};
void printn(hword val, hword color, hword bgcolor) {

	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i;
	
	// 入力値valの桁数判定
	tmp = val;
	for (i = 0; i < 5; i++) {
		if (tmp >= 10){
			tmp = div(tmp, 10);
		} else {
			break;
		}
	}

	// 文字列の最後にNULLコードを挿入
	buf[i+1] = 0;
	
	// 最下位桁の文字(10であった余り)を挿入し、入力値を10で割る
	for(; i >= 0; i--) {
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}
	
	// 文字列全体を表示
	prints(buf, color, bgcolor);
	
	return;
}


hword div(hword n, hword a){
	hword count=0;
    while(n-a>=0){
        n = n-a;
		count++;
    }
    return count;
}

hword mod(hword n, hword a){
    while(n-a>=0){
        n = n-a;
    }
    return n;
}


// 引数からコマンドを作成する
void make_command(hword a1, hword a2, hword a3, hword a4, hword a5){	
	command_number[0] = mod(a1, 8);
	a1/=8;
	command_number[1] = mod(a1, 8);
	command_number[2] = mod(a2, 8);
	a2/=8;
	command_number[3] = mod(a2, 8);
	command_number[4] = mod(a3, 8);
	a3/=8;
	command_number[5] = mod(a3, 8);
	command_number[6] = mod(a4, 8);
	a4/=8;
	command_number[7] = mod(a4, 8);
	command_number[8] = mod(a5, 8);
	a5/=8;
	command_number[9] = mod(a5, 8);

	for(k=0; k<10; k++){
		command_row[k] = commands_ascii[command_number[k]];
	}
	
}