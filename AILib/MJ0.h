/*
 * MJ0.h
 * �Ƃ������k����MJ-0 �A���S���Y�����������܂�(on �܂������IF)
 * �A���S���Y���̒��쌠�͂Ƃ������k�������L���܂�
 * �܂������̒��쌠�͐Δ������������L���܂�
 * �v���O�����̒��쌠��kacky�����L���܂�
 */
/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  �\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A�ȉ��̏����𖞂����ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B 

  �E�\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E�o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂��͔̔����i�ɁA�I�[�v�������̖��O�܂��̓R���g���r���[�^�[�̖��O���g�p���Ă͂Ȃ�Ȃ��B


  �{�\�t�g�E�F�A�́A���쌠�҂���уR���g���r���[�^�[�ɂ���āu����̂܂܁v�񋟂���Ă���A�����َ����킸�A
  ���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���K�����Ɋւ���Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�����܂���B
  ���쌠�҂��R���g���r���[�^�[���A���R�̂�������킸�A ���Q�����̌�����������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩
  �i�ߎ����̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ�������
  �i��֕i�܂��͑�p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A�܂�����Ɍ��肳��Ȃ��j
  ���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B 

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

		t_mentsu toTMentsu() const {
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
				return (p._nums[_num] >= 1.0 &&  p._nums[_num+1] >= 1.0 &&  p._nums[_num+2] >= 1.0) ? p._nums[_num] * p._nums[_num + 1] * p._nums[_num + 2] : 0.0;
			case MJAI::Mentsu::TYPE_KOUTSU:
				return p._nums[_num] >= 2.0 ? (p._nums[_num] - 2) * (p._nums[_num] - 2) : 0.0;
			case MJAI::Mentsu::TYPE_ATAMA:
				return p._nums[_num] >= 1.0 ? (p._nums[_num] - 1) * (p._nums[_num] - 1) * 0.6 : 0.0;
			default:
				break;
			}

			return 0.0f;
		}

		void add(std::vector<Pai> &array) const {
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				array.push_back(Pai(_num));
				array.push_back(Pai(_num+1));
				array.push_back(Pai(_num+2));
				break;
			case MJAI::Mentsu::TYPE_MINKAN:
			case MJAI::Mentsu::TYPE_ANKAN:
				array.push_back(Pai(_num));
			case MJAI::Mentsu::TYPE_KOUTSU:
				array.push_back(Pai(_num));
			case MJAI::Mentsu::TYPE_ATAMA:
				array.push_back(Pai(_num));
				array.push_back(Pai(_num));
				break;
			default:
				break;
			}
		}

		void add(PaiArray *p) const {
			switch (_type)
			{
			case MJAI::Mentsu::TYPE_SHUNTSU:
				p->_nums[_num]++;
				p->_nums[_num + 1]++;
				p->_nums[_num + 2]++;
				break;
			case MJAI::Mentsu::TYPE_KOUTSU:
				p->_nums[_num] += 3;
				break;
			case MJAI::Mentsu::TYPE_MINKAN:
			case MJAI::Mentsu::TYPE_ANKAN:
				p->_nums[_num] += 4;
				break;
			case MJAI::Mentsu::TYPE_ATAMA:
				p->_nums[_num] += 2;
				break;
			default:
				break;
			}
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

		MentsuType getType() const { return _type; }
		int getNum() const { return _num; }

		static std::vector<Mentsu> _all;
		static std::vector<Mentsu> _all_atama;

		static const std::vector<Mentsu> &all() {
			if (_all.empty())
			{
				for (int i = 0; i < 34; i++)
				{
					_all.push_back(Mentsu(TYPE_KOUTSU, i));
				}

				for (int i = 0; i < 7; i++)
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
			sum = std::accumulate(set.cbegin(), set.cend(), 0.0, [pai_kukan](double a, const Mentsu &m) { return a + m.weight(pai_kukan); });

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
		int _somete;
		double _rate;
		int _score;

		Player() :
			_is_riichi(false),
			_is_ippatsu(false),
			_rate(1.0),
			_score(0)
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
			_score = 0;
		}

		/* �e���p�C����m��(���L��) */
		double rate()
		{
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

		void calcSomete();
	};

	using Players = std::vector<Player>;
	using PaiArrayInt = int[34];

	/* ��v�Ɠ���̔v�Ƃ̋�����Ԃ�
	�z��͊��Ƀ\�[�g����Ă���Ɖ��肷��
	�z�񂩂����̔v�𔲂��o�����������ꍇ�A���̏ꏊ�ɂ�0x3F(34���傫����΂悢)������B

	��v�Ƃ̋����Ƃ�
	1)���v�̏ꍇ
	��v�̒��ł��̎�v�ɋ߂����v�̐��Ƃ̍��̐�Βl
	�Ⴆ�Έ��݂������Ă����Ƃ���

	��v�Ɉ��݂������0
	���݂������1
	�O�݂Ȃ�2
	�ƂȂ�
	��v���ݎq���Ȃ����-1(������)�ƂȂ�

	2)���v�̏ꍇ
	��v�ɓ������̂������Ă����0�A�Ȃ����-1(������)

	*/
	int paidistance(const std::vector<Pai>&tehai, int pai);

	class MJ0 {
	public:
		static void simulate(Players *players, Player *myself, std::vector<Pai> *doras);
	};
} // MJAI

