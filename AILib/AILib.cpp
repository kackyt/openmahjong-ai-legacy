
#include <functional>
#include "AILib.h"
#include "MJ0.h"

using namespace MJAI;


static int search_tahtsu(PaiArrayInt paiarray, TENPAI_LIST *machilist, int atamahai) {
	int i;
	int separate = 0;
	int numtah = 0;

	for (i = 0; i<size; i++) {
		if (paicountlist[i].count == 2) {
			machilist->machi[paicountlist[i].pai] = AI_FLAG_EFFECT_KOUTSU;
			separate = 1;
			numtah++;
		}
		else if (atamahai >= 34) {
			machilist->machi[paicountlist[i].pai] = AI_FLAG_EFFECT_ATAMA;
		}

		/* もう面子になっているものとの順子形成が残っているがそれは後で＾＾ */

		/* アタマ候補牌との順子形成 */
		if (atamahai <= 26 && (paicountlist[i].pai / 9 == atamahai / 9)) {
			if ((paicountlist[i].pai % 9) + 1 == (atamahai % 9)) {
				if ((paicountlist[i].pai % 9) == 0) {
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else if ((paicountlist[i + 1].pai % 9) == 8) {
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else {
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
			}
			else if ((paicountlist[i].pai % 9) + 2 == (atamahai % 9)) {
				machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
			}
			else if ((atamahai % 9) + 1 == (paicountlist[i].pai % 9)) {
				if ((atamahai % 9) == 0) {
					machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else if ((paicountlist[i].pai % 9) == 8) {
					machilist->machi[paicountlist[i].pai - 2] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else {
					machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 2] = AI_FLAG_EFFECT_SYUNTSU;
				}
			}
			else if ((atamahai % 9) + 2 == (paicountlist[i].pai % 9)) {
				machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
			}
		}

		if (i<size - 1) {
			if (paicountlist[i].pai > 26 || (paicountlist[i].pai / 9 != paicountlist[i + 1].pai / 9)) {
				separate = 0;
			}
			else if ((paicountlist[i].pai % 9) + 1 == (paicountlist[i + 1].pai % 9)) {
				if ((paicountlist[i].pai % 9) == 0) {
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else if ((paicountlist[i].pai % 9) == 8) {
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
				else {
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
				if (separate == 0) {
					numtah++;
					separate = 1;
				}
				else {
					separate = 0;
				}
			}
			else if ((paicountlist[i].pai % 9) + 2 == (paicountlist[i + 1].pai % 9)) {
				machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
				if (separate == 0) {
					numtah++;
					separate = 1;
				}
				else {
					separate = 0;
				}
			}

		}
		else {
			separate = 0;
		}

	}

	return numtah;
}


void search_mentsu(bool hasShuntsu, bool hasKoutsu, PaiArrayInt paiarray, std::vector<Mentsu> *pMentsu, std::vector<TENPAI_LIST> *pList, std::function<bool(std::vector<Mentsu> *pMentsu)> endFunction)
{
	if (pMentsu->size() == 5) {
		/* 上がってます */
		TENPAI_LIST list;
		memcpy(&list.mentsuflag, paiarray, sizeof(PaiArrayInt));

		list.shanten = 0;
		pList->push_back(list);
	} else if (pMentsu->size == 4) {
		/* アタマを探す */

	} else {
		if (hasShuntsu) {
			bool has = false;
			for (size_t i = 0; i < 21; i++) {
				int pai = (i / 7) * 9 + (i % 7);
				if (paiarray[pai] > 0 && paiarray[pai + 1] > 0 && paiarray[pai + 2] > 0) {
					has = true;
					paiarray[pai]--;
					paiarray[pai + 1]--;
					paiarray[pai + 2]--;
					pMentsu->push_back(Mentsu(Mentsu::MentsuType::TYPE_SHUNTSU, pai));
					search_mentsu(hasShuntsu, hasKoutsu, paiarray, pMentsu, pList, endFunction);
					paiarray[pai]++;
					paiarray[pai + 1]++;
					paiarray[pai + 2]++;
					pMentsu->pop_back();
				}
			}

			hasShuntsu = has;
		}

		if (hasKoutsu) {
			bool has = false;
			for (size_t pai = 0; pai < 34; pai++) {
				if (paiarray[pai] >= 3) {
					has = true;
					paiarray[pai] -= 3;
					pMentsu->push_back(Mentsu(Mentsu::MentsuType::TYPE_KOUTSU, pai));
					search_mentsu(hasShuntsu, hasKoutsu, paiarray, pMentsu, pList, endFunction);
					paiarray[pai] += 3;
					pMentsu->pop_back();
				}
			}

			hasKoutsu = has;
		}

		if (!hasShuntsu && !hasKoutsu) {
			/* アタマを探す */
			for (size_t pai = 0; pai < 34; pai++) {
				if (paiarray[pai] >= 2) {
					paiarray[pai] -= 2;
					pMentsu->push_back(Mentsu(Mentsu::MentsuType::TYPE_ATAMA, pai));
					
					paiarray[pai] += 3;
					pMentsu->pop_back();
				}
			}
		}
	}
}


int search_tenpai(const PaiArrayInt paiarray, PaiArrayInt pMachi, std::vector<TENPAI_LIST> *pList, int maxshanten)
{
}
