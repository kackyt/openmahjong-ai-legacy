/*
* MJ0.c
* とつげき東北氏のMJ-0 アルゴリズムを実装します(on まうじゃんIF)
* アルゴリズムの著作権はとつげき東北氏が所有します
* まうじゃんの著作権は石畑恭平氏が所有します
* プログラムの著作権はkackyが所有します
*/
/****************************************************************************************
* Copyright (c) 2016, Takaya Kakizaki(kacky)
* All rights reserved.

ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。

・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。

・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。

・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
（過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
（代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。

****************************************************************************************/


#include "MJ0.h"
#include <array>
#include <numeric>
#include <vector>

#define SIMU_SIZE (5000)

using namespace std;

namespace MJAI {
	vector<Mentsu> Mentsu::_all;
	vector<Mentsu> Mentsu::_all_atama;

	int paidistance(const std::vector<Pai>&tehai, int pai) {
		int i;
		unsigned prev = 0, n = 0;
		unsigned distp, distn;
		pai &= 0x3F;

		for (i = 0; i < tehai.size(); i++) {
			n = tehai[i].getNum() & 0x3F;
			/* 存在しない牌は飛ばす(手牌を抜き出す場合に便利) */
			if (n>33) continue;

			if (n >= pai) {
				if (pai >= 27) {
					/* 字牌 */
					return (n == pai) ? 0 : -1;
				}
				else if ((pai % 9) == 0) {
					/* 一萬、一索、一筒 */
					return (n - pai < 9) ? (n - pai) : -1;
				}
				else if ((pai % 9) == 8) {
					/* 九萬、九索、九筒 */
					return (pai - prev < 9) ? (pai - prev) : -1;
				}
				else {
					/* その他の牌 */

					/* 後ろの牌との距離(種類が違う場合は99) */
					if (n / 9 == pai / 9) {
						distn = n - pai;
					}
					else {
						distn = 99;
					}

					/* 前の牌との距離(種類が違う場合は99) */
					if (prev / 9 == pai / 9) {
						distp = pai - prev;
					}
					else {
						distp = 99;
					}

					/* 小さい方を返す */
					if (distn > distp) {
						return (distp < 9) ? distp : -1;
					}
					else {
						return (distn < 9) ? distn : -1;
					}
				}

			}
			prev = n;

		}

		/* 見つからない(つまり自分が一番後ろ) */
		if (pai >= 27) {
			/* 字牌 */
			return -1;
		}
		else if ((pai % 9) == 0) {
			/* 一萬、一索、一筒 */
			return -1;
		}
		else if ((pai % 9) == 8) {
			/* 九萬、九索、九筒 */
			return (pai - prev < 9) ? (pai - prev) : -1;
		}
		else {
			/* その他の牌 */

			/* 前の牌との距離(種類が違う場合は99) */
			if (prev / 9 == pai / 9) {
				distp = pai - prev;
			}
			else {
				distp = 99;
			}

			return (distp < 9) ? distp : -1;
		}

	}
	void MJ0::simulate(Players *players, Player *myself, vector<Pai> *doras)
	{
		PaiArray nokorihai(4.0f), pai_kukan;
		auto mentsu_full = [](Player &p) {return p._mentsu.size() + p._naki_mentsu.size() < 4; };

		for (auto &player : *players) {
			player._anpai = PaiArray();
			player._kikenhai = PaiArray();
			for (auto pai : player._tehai) {
				nokorihai[pai.getNum()] -= 1.0f;
			}

			for (auto pai : player._kawahai) {
				if (!pai.isNaki()) nokorihai[pai.getNum()] -= 1.0f;
				player._anpai[pai.getNum()] = 1.0f;
			}

			for (auto m : player._naki_mentsu) {
				m.sub(&nokorihai);
			}

			player._mentsu.clear();
		}

		myself->_kikenhai = PaiArray();

		for (auto dora : *doras) {
			nokorihai[dora.getDorahyouji()] -= 1.0f;
		}

		for (size_t i = 0; i < SIMU_SIZE; i++) {
			for (auto &player : *players) {
				player._pai_kukan = nokorihai;
			}

			myself->_pai_kukan = nokorihai;

			Players::iterator it;
			while ((it = find_if(players->begin(), players->end(), mentsu_full))
				   != players->end()) {
				while ((it = find_if(it, players->end(), mentsu_full))
					   != players->end()) {
					auto all = Mentsu::all();
					const auto &pai_kukan = it->_pai_kukan;
					// メンツをランダムで抽出する
					float sum = accumulate(all.cbegin(), all.cend(), 0.0f, [pai_kukan](float a,const Mentsu &m) { return a + m.weight(pai_kukan); });
					float val = rand() * sum / RAND_MAX;
					float tmp = 0.0f;
					auto mentsu = find_if(all.cbegin(), all.cend(), [val, tmp, pai_kukan](const Mentsu &m) mutable { tmp += m.weight(pai_kukan); return val < tmp; });
					(it->_mentsu).push_back(*mentsu);

					// メンツを構成する牌を牌空間から除去する
					for (auto &p2 : *players)
					{
						mentsu->sub(&p2._pai_kukan);
					}
					mentsu->sub(&myself->_pai_kukan);

					// 牌の危険度を更新する
					mentsu->addKiken(&it->_kikenhai);
					mentsu->addKiken(&myself->_kikenhai);
					it++;
				}
			}

			for (auto &player : *players) {
				auto all = Mentsu::all_atama();
				const auto &pai_kukan = player._pai_kukan;
				// アタマをランダムで抽出する
				float sum = accumulate(all.cbegin(), all.cend(), 0.0f, [pai_kukan](float a, const Mentsu &m) { return a + m.weight(pai_kukan); });
				float val = rand() * sum / RAND_MAX;
				float tmp = 0.0f;
				auto mentsu = find_if(all.cbegin(), all.cend(), [val, tmp, pai_kukan](const Mentsu &m) mutable { tmp += m.weight(pai_kukan); return val <= tmp; });
				(player._mentsu).push_back(*mentsu);

				// アタマを構成する牌を牌空間から除去する
				for (auto &p2 : *players)
				{
					mentsu->sub(&p2._pai_kukan);
				}
				mentsu->sub(&myself->_pai_kukan);

				// 牌の危険度を更新する
				mentsu->addKiken(&player._kikenhai);
				mentsu->addKiken(&myself->_kikenhai);
			}

			pai_kukan += myself->_pai_kukan;
		}

		myself->_pai_kukan = pai_kukan / SIMU_SIZE;
		myself->_kikenhai = myself->_kikenhai / SIMU_SIZE;
	}
} // MJAI
