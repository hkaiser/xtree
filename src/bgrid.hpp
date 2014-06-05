/*
 * bgrid.hpp
 *
 *  Created on: May 29, 2014
 *      Author: dmarce1
 */

#ifndef BGRID_HPP_
#define BGRID_HPP_

#include "fwd.hpp"

namespace xtree {
template<typename T, typename Dims, int Bw>
class bgrid: public grid_base<T, Dims::dim()> {
public:
	static constexpr int Ndim = Dims::dim();
	using grid_type = grid<T, Dims, Bw>;
	using index_type = typename grid_base<T, Ndim>::index_type;
private:
	const dir_type<Ndim> dir;
	int size;
	std::shared_ptr<const grid_type> local_ptr;
	std::vector<T> data;
public:
	bgrid() {
	}
	bgrid(const dir_type<Ndim>& _dir, const std::shared_ptr<const grid_type>& lptr) :
			dir(_dir), local_ptr(lptr), data(0) {
	}
	virtual ~bgrid() {
	}
	bgrid(bgrid<T, Dims, Bw> && bg) :
			dir(std::move(bg.dir)) {
		size = bg.size;
		data = std::move(bg.data);
		local_ptr = std::move(local_ptr);
		bg.local_ptr = nullptr;
	}
	template<typename Arc>
	void load(Arc& ar, const int v) {
		local_ptr = nullptr;
		size = 1;
		for (int i = 0; i < Ndim; i++) {
			if (dir[i] == 0) {
				size *= Dims::get(i);
			} else {
				size *= Bw;
			}
		}
		data.resize(size);
		data.load(ar, v);
	}
	template<typename Arc>
	void save(Arc& ar, const int v) const {
		if (local_ptr) {
			vector<int, Ndim> min, max;
			for (int d = 0; d < Ndim; d++) {
				switch (dir[d]) {
				case -1:
					min[d] = Dims::get(d) - Bw;
					max[d] = Dims::get(d);
					break;
				case 0:
					min[d] = 0;
					max[d] = Dims::get(d);
					break;
				case +1:
					min[d] = 0;
					max[d] = Bw;
					break;
				}
			}
			for (grid_index<Ndim> i(min, max); !i.end(); ++i) {
				ar << local_ptr->get(i);
			}
		} else {
			data.save(ar, v);
		}
	}
	const T& get(const index_type& i) const {
		if (local_ptr) {
			return (*local_ptr)[i];
		} else {
			int index = 0;
			for (int d = Ndim - 1; d >= 0; d--) {
				switch (dir[d]) {
				case -1:
					index *= Bw;
					index += i[d] + Bw - Dims::get(d);
					break;
				case 0:
					index *= Dims::get(d);
					index += i[d];
					break;
				case +1:
					index *= Bw;
					index += i[d];
					break;
				}
			}
			return data[index];
		}
	}
};

}

#endif /* BGRID_HPP_ */
