#pragma once
#include <cassert>
#include <vector>
#include <array>
#include <numeric>
#include <functional> 

template <typename T, int ND>
class MDArray
{
public:
	inline static constexpr int ndim = ND;

	using index_type = std::array<int, ndim>;

	//默认构造函数
	MDArray() = default;

	MDArray(const MDArray&) = default;

	// 构造函数
	template<typename ... Args>
	MDArray(Args...args) :data_((1*...*args)), size_{ args... }
	{
		static_assert(sizeof...(Args) == ndim);
		update_stride();
	};

	MDArray(const index_type& size)
		:data_(std::accumulate(size.begin(), size.end(), 1, std::multiplies<int>())),
		size_(size)
	{
		update_stride();
	}

	void clear()
	{
		data_.clear();
		size_.fill(0);
		stride_.fill(0);
	}

	MDArray& operator =(const MDArray&) = default;

	// 动态改变数组尺寸
	template<typename ... Args>
	void resize(Args...args)
	{
		//size_.assign({ args... });
		size_ = { args... };
		update_stride();
		data_.resize((args*...));
	}

	void resize(const index_type& new_size)
	{
		size_ = new_size;
		update_stride();
		data_.resize(std::accumulate(size_.begin(), size_.end(), 1, std::multiplies<int>()));
	}

	// 访问元素值的方法：括号()运算符
	template<typename ... Args>
	T& operator()(Args ...args)
	{
		static_assert(sizeof...(Args) == ndim);
		return data_[map_1d(args...)];
	}
	template<typename ... Args>
	const T& operator()(Args ...args)const
	{
		static_assert(sizeof...(Args) == ndim);
		return data_[map_1d(args...)];
	}

	T& operator()(const index_type& id)
	{
		return data_[map_1d(id)];
	}
	const T& operator()(const index_type& id)const
	{
		return data_[map_1d(id)];
	}

	// 数组的维度个数
	constexpr int rank()const { return ndim; }

	// 数组指定维度的尺寸
	int extent(int idim)const
	{
		assert(idim >= 0 && idim < ndim);
		return size_[idim];
	}

	const index_type& extent()const { return size_; }

	int stride(int idim)const
	{
		assert(idim >= 0 && idim < ndim);
		return stride_[idim];
	}

	const index_type& stride()const { return stride_; }

	template<typename ... Args>
	int map_1d(Args ...args)const
	{
		return map_1d_imp(args...);
	}
	int map_1d(const index_type& id)const
	{
		for (int i = 0; i < ndim; ++i)assert(id[i] >= 0 && id[i] < size_[i]);
		return std::inner_product(id.begin(), id.end(), stride_.begin(), 1);
	}
private:
	void update_stride()
	{
		stride_.back() = 1;
		for (int i = 1; i < ndim; ++i) {
			stride_[ndim - i - 1] = size_[ndim - i] * stride_[ndim - i];
		}
	}
	template<typename ... Args>
	int map_1d_imp(int i, Args ...args)const
	{
		constexpr auto idim = ndim - 1 - sizeof...(args);
		assert(i >= 0 && i < std::get<idim>(size_));
		if constexpr (sizeof...(args) == 0)
			return i * std::get<ndim - 1>(stride_);
		else
			return i * std::get<idim>(stride_) + map_1d_imp(args...);
	}

private:
	// 内部用动态数组存储数据 ： std::vector
	std::vector<T> data_;

	// 存储数组每一维度的尺寸
	index_type size_{ 0 };

	// 跨距
	index_type stride_{ 0 };
};
