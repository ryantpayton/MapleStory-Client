//////////////////////////////////////////////////////////////////////////////
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
#include <functional>
#include <vector>
#include <unordered_map>

using std::function;
using std::vector;
using std::unordered_map;

template<typename K, typename V>
class BinaryTree
{
public:
	BinaryTree()
	{
		root = 0;
	}

	void add(K key, V value)
	{
		nodes[key] = { value, 0, 0 };

		if (root)
		{
			K current = root;
			while (current)
			{
				current = nodes[current].addornext(key, value);
			}
		}
		else
		{
			root = key;
		}
	}

	K min()
	{
		K current = root;
		K previous;
		while (current)
		{
			previous = current;
			current = nodes[current].left;
		}
		return previous;
	}

	K max()
	{
		K current = root;
		K previous;
		while (current)
		{
			previous = current;
			current = nodes[current].right;
		}
		return previous;
	}

	V& operator [](K key)
	{
		return nodes[key].value;
	}

	const V& operator [](K key) const
	{
		return nodes.at(key).value;
	}

	void minwalk(function<void(const K&)> action)
	{
		minwalkfrom(root, action);
	}

	void inorderwalk(function<void(const K&)> action)
	{
		inorderwalkfrom(root, action);
	}

	void maxwalk(function<void(const K&)> action)
	{
		maxwalkfrom(root, action);
	}

private:
	void minwalkfrom(K current, function<void(const K&)> action)
	{
		if (current)
		{
			minwalkfrom(nodes[current].left, action);
			action(current);
			minwalkfrom(nodes[current].right, action);
		}
	}

	void inorderwalkfrom(K current, function<void(const K&)> action)
	{
		if (current)
		{
			action(current);
			inorderwalkfrom(nodes[current].left, action);
			inorderwalkfrom(nodes[current].right, action);
		}
	}

	void maxwalkfrom(K current, function<void(const K&)> action)
	{
		if (current)
		{
			maxwalkfrom(nodes[current].right, action);
			action(current);
			maxwalkfrom(nodes[current].left, action);
		}
	}

	struct Node
	{
		V value;
		K left;
		K right;

		K addornext(K key, V val)
		{
			if (val >= value)
			{
				if (right)
				{
					return right;
				}
				else
				{
					right = key;
					return 0;
				}
			}
			else
			{
				if (left)
				{
					return left;
				}
				else
				{
					left = key;
					return 0;
				}
			}
		}
	};

	unordered_map<K, Node> nodes;
	K root;
};