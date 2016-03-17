//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <functional>

template <typename T>
class Interpolated
{
public:
	Interpolated()
	{
		set(T());
	}

	void set(T value)
	{
		now = value;
		before = value;
	}

	void normalize()
	{
		before = now;
	}

	T last() const
	{
		return before;
	}

	virtual T get() const = 0;

	virtual T get(float alpha) const = 0;

protected:
	T now;
	T before;
};

template <typename T>
class Nominal : public Interpolated<T>
{
public:
	Nominal()
	{
		threshold = 0.0f;
	}

	T get() const
	{
		return now;
	}

	T get(float alpha) const
	{
		return alpha >= threshold ? now : before;
	}

	void next(T value, float thrs)
	{
		before = now;
		now = value;
		threshold = thrs;
	}

	bool operator == (T value) const
	{
		return now == value;
	}

	bool operator != (T value) const
	{
		return now != value;
	}

	T operator + (T value) const
	{
		return now + value;
	}

	T operator - (T value) const
	{
		return now - value;
	}

	T operator * (T value) const
	{
		return now * value;
	}

	T operator / (T value) const
	{
		return now / value;
	}

private:
	float threshold;
};

template <typename T>
class Linear : public Interpolated<T>
{
public:
	T get() const
	{
		return now;
	}

	T get(float alpha) const
	{
		return static_cast<T>((1.0f - alpha) * before + alpha * now);
	}

	void operator = (T value)
	{
		before = now;
		now = value;
	}

	void operator += (T value)
	{
		before = now;
		now += value;
	}

	void operator -= (T value)
	{
		before = now;
		now -= value;
	}

	bool operator == (T value) const
	{
		return now == value;
	}

	bool operator != (T value) const
	{
		return now != value;
	}

	bool operator < (T value) const
	{
		return now < value;
	}

	bool operator <= (T value) const
	{
		return now <= value;
	}

	bool operator > (T value) const
	{
		return now > value;
	}

	bool operator >= (T value) const
	{
		return now >= value;
	}

	T operator + (T value) const
	{
		return now + value;
	}

	T operator - (T value) const
	{
		return now - value;
	}

	T operator * (T value) const
	{
		return now * value;
	}

	T operator / (T value) const
	{
		return now / value;
	}

	T operator + (Linear<T> value) const
	{
		return now + value.get();
	}

	T operator - (Linear<T> value) const
	{
		return now - value.get();
	}

	T operator * (Linear<T> value) const
	{
		return now * value.get();
	}

	T operator / (Linear<T> value) const
	{
		return now / value.get();
	}
};