/*
 * MJ0.h
 * とつげき東北氏のMJ-0 アルゴリズムを実装します(on まうじゃんIF)
 * アルゴリズムの著作権はとつげき東北氏が所有します
 * まうじゃんの著作権は石畑恭平氏が所有します
 * プログラムの著作権はkackyが所有します
 */
/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
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
#pragma once

#include <windows.h>
#include "MIPIface.h"
#include "AILib.h"

#define SIMU_SIZE (5000)

#include <vector>
#include <algorithm>
#include <numeric>

namespace MJAI {

	class PaiArray {
	public:
		double _nums[34];

		PaiArray(double iniVal = 0.0f) {
			fill(iniVal);
		}

		void fill(double val) {
			for (int i = 0; i < 34; i++)
			{
				_nums[i] = val;
			}
		}

		size_t size() {
			return 34;
		}

		PaiArray inverse() {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				if (_nums[i] > 0)
				{
					ret._nums[i] = 0;
				}
				else {
					ret._nums[i] = 1.0;
				}
			}

			return ret;
		}

		PaiArray partialMultiple(int category, double rate) {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				if ((i / 9) == category)
				{
					ret._nums[i] = _nums[i] * rate;
				}
				else {
					ret._nums[i] = _nums[i];
				}
			}

			return ret;
		}

		PaiArray& operator =(const PaiArray &other) {

			for (int i = 0; i < 34; i++)
			{
				_nums[i] = other._nums[i];
			}

			return *this;
		}

		PaiArray &operator +=(const PaiArray &other) {

			for (int i = 0; i < 34; i++)
			{
				_nums[i] += other._nums[i];
			}

			return *this;
		}

		PaiArray operator +(const PaiArray &other) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] + other._nums[i];
			}

			return ret;
		}

		PaiArray operator +(const double val) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] + val;
			}

			return ret;
		}

		PaiArray operator -(const double val) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] - val;
			}

			return ret;
		}

		PaiArray operator /(const double val) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] / val;
			}

			return ret;
		}

		PaiArray operator *(const double val) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] * val;
			}

			return ret;
		}

		PaiArray operator *(const PaiArray &val) const {
			PaiArray ret;

			for (int i = 0; i < 34; i++)
			{
				ret._nums[i] = _nums[i] * val._nums[i];
			}

			return ret;
		}

		double & operator[](const int idx) {
			return _nums[idx];
		}
	};

	class Mentsu {
	public:
		enum MentsuType {
			TYPE_SHUNTSU = AI_SYUNTSU,
			TYPE_KOUTSU = AI_KOUTSU,
			TYPE_MINKAN = AI_MINKAN,
			TYPE_ANKAN = AI_ANKAN,
			TYPE_ATAMA = AI_ATAMA
		};

		Mentsu(MentsuType type = TYPE_SHUNTSU, int num = 0) :
			_type(type),
			_num(num)
		{}

		t_mentsu toTMentsu() {
			t_mentsu ret;
			ret.category = static_cast<int>(_type);
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				ret.pailist[0] = _num;
				ret.pailist[1] = _num + 1;
				ret.pailist[2] = _num + 2;
				break;
			default:
				ret.pailist[0] = _num;
				ret.pailist[1] = _num;
				ret.pailist[2] = _num;
				break;
			}

			return ret;
		}

		void addKiken(PaiArray *p) const {
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				if ((_num % 9) > 0) p->_nums[_num - 1] += 0.1f;
				p->_nums[_num] += 0.2f;
				p->_nums[_num + 1] += 0.1f;
				p->_nums[_num + 2] += 0.2f;
				if ((_num % 9) < 6) p->_nums[_num + 3] += 0.1f;
				break;
			case MJAI::Mentsu::TYPE_KOUTSU:
				p->_nums[_num] += 0.05f;
				break;
			case MJAI::Mentsu::TYPE_ATAMA:
				p->_nums[_num] += 0.1f;
				break;
			default:
				break;
			}
		}

		double weight(const PaiArray &p) const {
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				return p._nums[_num] * p._nums[_num + 1] * p._nums[_num + 2];
			case MJAI::Mentsu::TYPE_KOUTSU:
				return p._nums[_num] >= 2.0f ? (p._nums[_num] - 2) * (p._nums[_num] - 2) : 0.0f;
			case MJAI::Mentsu::TYPE_ATAMA:
				return p._nums[_num] >= 1.0f ? (p._nums[_num] - 1) * (p._nums[_num] - 1) * 0.6f : 0.0f;
			default:
				break;
			}

			return 0.0f;
		}

		void sub(PaiArray *p) const {
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				p->_nums[_num]--;
				p->_nums[_num + 1]--;
				p->_nums[_num + 2]--;
				break;
			case MJAI::Mentsu::TYPE_KOUTSU:
				p->_nums[_num] -= 3;
				break;
			case MJAI::Mentsu::TYPE_MINKAN:
			case MJAI::Mentsu::TYPE_ANKAN:
				p->_nums[_num] -= 4;
				break;
			case MJAI::Mentsu::TYPE_ATAMA:
				p->_nums[_num] -= 2;
				break;
			default:
				break;
			}
		}

		MentsuType getType() { return _type; }
		int getNum() { return _num; }

		static std::vector<Mentsu> _all;
		static std::vector<Mentsu> _all_atama;

		static const std::vector<Mentsu> &all() {
			if (_all.empty())
			{
				for (int i = 0; i < 34; i++)
				{
					_all.push_back(Mentsu(TYPE_KOUTSU, i));
				}

				for (int i = 0; i < 8; i++)
				{
					_all.push_back(Mentsu(TYPE_SHUNTSU, i));
					_all.push_back(Mentsu(TYPE_SHUNTSU, i + 9));
					_all.push_back(Mentsu(TYPE_SHUNTSU, i + 18));
				}
			}


			return _all;
		}

		static const std::vector<Mentsu> &all_atama() {
			if (_all_atama.empty())
			{
				for (int i = 0; i < 34; i++)
				{
					_all_atama.push_back(Mentsu(TYPE_ATAMA, i));
				}

			}

			return _all_atama;
		}
		static const Mentsu& sample(const std::vector<Mentsu> &set, const PaiArray &pai_kukan) {
			double sum = 0.0f;
			double it = 0.0f;
			sum = accumulate(set.cbegin(), set.cend(), 0.0f, [pai_kukan](double a, const Mentsu &m) { return a + m.weight(pai_kukan); });

			double r = rand() * sum / RAND_MAX;

			auto smpl = std::find_if(set.cbegin(), set.cend(), [r, it, pai_kukan](const Mentsu &m) mutable {
				it += m.weight(pai_kukan);
				return r < it;
			});

			if (smpl == set.cend())
			{
				smpl--;
			}

			return *smpl;
		}

	private:
		MentsuType _type;
		int _num;
	};

	class Pai {
	public:
		Pai(int num = 0, bool aka = false, bool naki = false, bool riichi = false) :
			_num(num & 63),
			_aka(aka),
			_naki(naki),
			_riichi(riichi)
		{
			if (num & 64) {
				_aka = true;
			}
		}

		int getNum() const { return _num; }
		bool isAka() const { return _aka; }
		bool isNaki() const { return _naki; }
		bool isRiichi() const { return _riichi; }

		int getDorahyouji()
		{
			if (_num < 27) {
				if ((_num % 9) == 0) {
					return _num + 8;
				}
				else {
					return _num - 1;
				}
			}
			else if (_num == 27) {
				return 30;
			}
			else if (_num == 31) {
				return 33;
			}
			else {
				return _num - 1;
			}
		}
	private:
		int _num;
		bool _aka;
		bool _naki;
		bool _riichi;
	};

	class Player {
	public:
		std::vector<Mentsu> _mentsu;
		std::vector<Mentsu> _naki_mentsu;
		PaiArray _pai_kukan;
		PaiArray _anpai;
		PaiArray _kikenhai;
		std::vector<Pai> _tehai;
		std::vector<Pai> _kawahai;
		bool _is_riichi;
		bool _is_ippatsu;
		double _rate;

		Player() :
			_is_riichi(false),
			_is_ippatsu(false),
			_rate(1.0)
		{

		}

		void clear() {
			_mentsu.clear();
			_naki_mentsu.clear();
			_pai_kukan = PaiArray();
			_anpai = PaiArray();
			_kikenhai = PaiArray();
			_tehai.clear();
			_kawahai.clear();
			_is_riichi = false;
			_is_ippatsu = false;
		}

		double rate()
		{
			/* テンパイする確率(鳴き有り) */
			static float tempai_table[] = {
				0.0,
				0.001,
				0.003,
				0.018,
				0.053,
				0.105,
				0.197,
				0.328,
				0.467,
				0.587,
				0.705,
				0.772,
				0.839,
				0.886,
				0.917,
				0.937,
				0.952,
				0.960,
				1.0,
				1.0,
				1.0,
				1.0
			};

			if (_is_riichi)
			{
				return 1.0;
			}
			else {
				return tempai_table[_kawahai.size()];
			}
		}

		bool isMenzen() {
			for (auto mentsu : _naki_mentsu) {
				if (mentsu.getType() != Mentsu::TYPE_ANKAN) {
					return false;
				}
			}

			return true;
		}

		PaiArray kikenhai() {
			return _kikenhai * _anpai.inverse() * rate();
		}

		void fromTehai(const MJITehai *tehai) {
			_tehai.clear();
			_naki_mentsu.clear();

			if (!(tehai->tehai_max > 4 && tehai->tehai[tehai->tehai_max-1] == 0))
			{
				for (size_t i = 0; i < tehai->tehai_max; i++) {
					_tehai.push_back(Pai(tehai->tehai[i]));
				}
			}

			for (size_t i = 0; i < tehai->ankan_max; i++) {
				_naki_mentsu.push_back(Mentsu(Mentsu::TYPE_ANKAN, tehai->ankan[i]));
			}
			for (size_t i = 0; i < tehai->minkan_max; i++) {
				_naki_mentsu.push_back(Mentsu(Mentsu::TYPE_MINKAN, tehai->minkan[i]));
			}
			for (size_t i = 0; i < tehai->minshun_max; i++) {
				_naki_mentsu.push_back(Mentsu(Mentsu::TYPE_SHUNTSU, tehai->minshun[i]));
			}
			for (size_t i = 0; i < tehai->minkou_max; i++) {
				_naki_mentsu.push_back(Mentsu(Mentsu::TYPE_KOUTSU, tehai->minkou[i]));
			}
		}

		void toTehai(MJITehai *tehai) {
			tehai->tehai_max = 0;
			tehai->ankan_max = 0;
			tehai->minkan_max = 0;
			tehai->minshun_max = 0;
			tehai->minkou_max = 0;
			for (auto pai : _tehai) {
				tehai->tehai[tehai->tehai_max++] = pai.getNum();
			}
		}

		void fromKawahai(const MJIKawahai *kawa, size_t num) {
			_kawahai.clear();
			for (size_t i = 0; i < num; i++) {
				_kawahai.push_back(Pai(kawa[i].hai, false, kawa[i].state & MJKS_NAKI, kawa[i].state & MJKS_REACH));
			}
		}
	};

	using Players = std::vector<Player>;
	using PaiArrayInt = int[34];

	/* 手牌と特定の牌との距離を返す
	配列は既にソートされていると仮定する
	配列から特定の牌を抜き出す操作をする場合、その場所には0x3F(34より大きければよい)を入れる。

	手牌との距離とは
	1)数牌の場合
	手牌の中でその手牌に近い数牌の数との差の絶対値
	例えば一萬を持っていたとして

	手牌に一萬があれば0
	二萬があれば1
	三萬なら2
	となる
	手牌に萬子がなければ-1(無限大)となる

	2)字牌の場合
	手牌に同じものを持っていれば0、なければ-1(無限大)

	*/
	int paidistance(const std::vector<Pai>&tehai, int pai);

	class MJ0 {
	public:
		static void simulate(Players *players, Player *myself, std::vector<Pai> *doras);
	};
} // MJAI

