/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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

template <typename T>
class Consumer
{
public:
	Consumer(void(*f)(T))
	{
		fun = f;
	}

	Consumer()
	{
		fun = nullptr;
	}

	~Consumer() {}

	bool available()
	{
		return fun != nullptr;
	}

	void accept(T arg)
	{
		if (available())
			fun(arg);
	}

private:
	void(*fun)(T);
};

template <typename R, typename A>
class Function
{
public:
	Function(R(*f)(A))
	{
		fun = f;
	}

	Function(R nv)
	{
		fun = nullptr;
		nullvalue = nv;
	}

	~Function() {}

	bool available()
	{
		return fun != nullptr;
	}

	R apply(A arg)
	{
		if (available())
			return fun(arg);
		else
			return nullvalue;
	}

private:
	R(*fun)(A);
	R nullvalue;
};

template <typename R>
class Supplier
{
public:
	Supplier(R(*f)())
	{
		fun = f;
	}

	Supplier(R nv)
	{
		fun = nullptr;
		nullvalue = nv;
	}

	~Supplier() {}

	bool available()
	{
		return fun != nullptr;
	}

	R get()
	{
		if (available())
			return fun();
		else
			return nullvalue;
	}

private:
	R(*fun)();
	R nullvalue;
};