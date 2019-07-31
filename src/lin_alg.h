#ifndef LIN_ALG_H
#define LIN_ALG_H

#include <cblas.h>

namespace LinAlg {
template<typename T> concept bool DATATYPE_INTEGER_Contract = requires
{
	std::is_integral<T>::value;
};

template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
};

template<size_t D> concept bool DIMENSION_Contract = requires(size_t d) 
{
	{ d > 0 };
};

template<size_t M, size_t N, typename T> requires
(DATATYPE_FLOATING_Contract<T> || DATATYPE_INTEGER_Contract<T>) &&
DIMENSION_Contract<M> &&
DIMENSION_Contract<N>
class Mat;

template<size_t D, typename T = float> requires
(DATATYPE_FLOATING_Contract<T> || DATATYPE_INTEGER_Contract<T>) &&
DIMENSION_Contract<D>
class Vec
{
public:
	Vec() {}
	Vec(const Vec<D, T>& rhs) : m_data(rhs.m_data) {}
	Vec(Vec<D, T>&& rhs) : m_data(std::move(rhs.m_data)) {}
	Vec(T x) : m_data({x}) {}
	Vec(T* ptr) : m_data(ptr) {}
	template<size_t ... I>
	Vec(const T* const ptr, std::index_sequence<I ...> index) : m_data((ptr[I], ...)) {}
	Vec(const T* const ptr) : Vec(ptr, std::make_index_sequence<D>{}) {}
	template<typename ... Args> requires
	((DATATYPE_FLOATING_Contract<Args> || DATATYPE_INTEGER_Contract<Args>) && ...)
	Vec(Args ... values) : m_data({(values, ...)}) 
	{
		static_assert(sizeof ...(values) == D);
	}
public:
	T* data() { return m_data.data(); }
	const T* const data() const { return m_data.data(); }
	T& operator[](size_t i) { return m_data[i]; }
	const T& operator[](size_t i) const { return m_data[i]; }
public:
	T dot(const Vec<D, T>& rhs) const
	{
		if constexpr (std::is_same<T, float>::value) {
			return cblas_sdot(D, data(), 1, rhs.data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			return cblas_ddot(D, data(), 1, rhs.data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
	}
	T length() const
	{
		if constexpr (std::is_same<T, float>::value) {
			return cblas_snrm2(D, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			return cblas_dnrm2(D, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
	}
	T lengthSquared() const
	{
		return dot(*this);
	}
	T distance(const Vec<D, T>& rhs) const
	{
		return (*this-rhs).length();
	}
	T distanceSquared(const Vec<D, T>& rhs) const
	{
		return (*this-rhs).lengthSquared();
	}
	Vec<D, T>& normalize()
	{
		if constexpr (std::is_same<T, float>::value) {
			(1.0f/length())*(*this);
		}
		else if constexpr (std::is_same<T, double>::value) {
			(1.0/length())*(*this);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return *this;
	}
	T min() const
	{
		if constexpr (std::is_same<T, float>::value) {
			return cblas_isamin(D, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			return cblas_dsamin(D, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
	}
	T max() const
	{
		if constexpr (std::is_same<T, float>::value) {
			return cblas_isamax(D, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			return cblas_dsamax(D, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
	}
	Vec<D, T> min(const Vec<D, T>& rhs) const
	{
		return std::apply([this, rhs]<size_t ... I>(std::index_sequence<I ...>){
			return Vec<D, T>((std::min(this->m_data[I], rhs.m_data[I]), ...));
		}, std::make_index_sequence<D>{});
	}
	Vec<D, T> max(const Vec<D, T>& rhs) const
	{
		return std::apply([this, rhs]<size_t ... I>(std::index_sequence<I ...>){
			return Vec<D, T>((std::max(this->m_data[I], rhs.m_data[I]), ...));
		}, std::make_index_sequence<D>{});
	}
	Vec<D, T> cross(const Vec<D, T>& rhs)
	{
		static_assert(D == 3);
		if constexpr (std::is_same<T, float>::value) {
			Mat<D, D, T> helper({{0.0f, m_data[2], -m_data[1]}, {-m_data[2], 0.0f, m_data[0]}, {m_data[1], -m_data[0], 0.0f}});
			return helper*rhs;
		}
		else if constexpr (std::is_same<T, double>::value) {
			Mat<D, D, T> helper({{0.0, m_data[2], -m_data[1]}, {-m_data[2], 0.0, m_data[0]}, {m_data[1], -m_data[0], 0.0}});
			return helper*rhs;
		}
		else {
			throw std::logic_error("Not implemented");
		}
	}
	Vec<D, T>& operator+(const Vec<D, T>& rhs)
	{
		if constexpr (std::is_same<T, float>::value) {
			cblas_saxpy(D, 1.0f, rhs.data(), 1, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			cblas_daxpy(D, 1.0, rhs.data(), 1, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return *this;
	}
	Vec<D, T>& operator-(const Vec<D, T>& rhs)
	{
		if constexpr (std::is_same<T, float>::value) {
			cblas_saxpy(D, -1.0f, rhs.data(), 1, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			cblas_daxpy(D, -1.0, rhs.data(), 1, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return *this;
	}
	Vec<D, T>& operator*(const T value)
	{
		if constexpr (std::is_same<T, float>::value) {
			cblas_sscal(D, value, data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			dblas_dscal(D, value, data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return *this;
	}
protected:
	std::array<T, D> m_data;
};

template<size_t D, typename T = float> requires
(DATATYPE_FLOATING_Contract<T> || DATATYPE_INTEGER_Contract<T>) &&
DIMENSION_Contract<D>
class BBox
{
public:
	BBox() {}
	BBox(const BBox<D, T>& rhs) : m_min(rhs.m_min), m_max(rhs.m_max) {}
	BBox(const BBox<D, T>& rhs, T margin) : m_min(rhs.min() - Vec<D, T>(margin)), m_max(rhs.max() + Vec<D, T>(margin)) {} 
	BBox(BBox<D, T>&& rhs) : m_min(std::move(rhs.m_min)), m_max(std::move(rhs.m_max)) {}
	BBox(const Vec<D, T>& min, const Vec<D, T> max) : m_min(min), m_max(max) {}
	BBox(Vec<D, T>&& min, const Vec<D, T> max) : m_min(std::move(min)), m_max(max) {}
	BBox(const Vec<D, T>& min, Vec<D, T>&& max) : m_min(min), m_max(std::move(max)) {}
	BBox(Vec<D, T>&& min, Vec<D, T>&& max) : m_min(std::move(min)), m_max(std::move(max)) {}
public:
	Vec<D, T>& min() { return m_min; }
	Vec<D, T>& max() { return m_max; }
	T* data() { return m_min.data(); }
	const T* const data() const { return m_min.data(); }
public: // TODO: Move these to separate context.
	void engulf(const Vec<D, T>& rhs)
	{
		m_min = m_min.min(rhs);
		m_max = m_max.max(rhs);
	}
	void engulf(const BBox<D, T>& rhs)
	{
		m_min = m_min.min(rhs.m_min);
		m_max = m_max.max(rhs.m_max);
	}
	T diagonal() const
	{
		return m_min.distance(m_max);
	}
	bool isEmpty() const
	{
		return m_max[0] < m_min[0];
	}
	bool isNotEmpty() const
	{
		return m_min[0] <= m_max[0];
	}
	T maxSideLength() const
	{
		return (m_max - m_min).max();
	}
	bool isStrictlyInside(const BBox<D, T>& other)
	{
		return std::apply([this, other]<size_t ... I>(std::index_sequence<I...>){
			return ((this->m_min[I] < other.m_min[I]) && ...) && ((this->m_max[I] < other.m_max[I]) && ...);
		}, std::make_index_sequence<D>{});
	}
	bool isNotOverlapping(const BBox<D, T>& other)
	{
		return std::apply([this, other]<size_t ... I>(std::index_sequence<I...>){
			return ((other.m_max[I] < this->m_min[I]) || ...) || ((this->m_max[I] < other.m_min[I]) || ...);
		}, std::make_index_sequence<D>{});
	}
	bool isOverlapping(const BBox<D, T>& other) 
	{ 
		return !isNotOverlapping(other); 
	}
	//BBox3f transform(const Mat3x4f& M, const BBox3f& bbox)
	//{
		//throw std::logic_error("Not implemented");
		/*Vec3f p[8] = {
		mul(M, Vec3f(bbox.min.x, bbox.min.y, bbox.min.z)),
		mul(M, Vec3f(bbox.min.x, bbox.min.y, bbox.max.z)),
		mul(M, Vec3f(bbox.min.x, bbox.max.y, bbox.min.z)),
		mul(M, Vec3f(bbox.min.x, bbox.max.y, bbox.max.z)),
		mul(M, Vec3f(bbox.max.x, bbox.min.y, bbox.min.z)),
		mul(M, Vec3f(bbox.max.x, bbox.min.y, bbox.max.z)),
		mul(M, Vec3f(bbox.max.x, bbox.max.y, bbox.min.z)),
		mul(M, Vec3f(bbox.max.x, bbox.max.y, bbox.max.z))
		};
		return BBox3f(min(min(min(p[0], p[1]), min(p[2], p[3])), min(min(p[4], p[5]), min(p[6], p[7]))),
			max(max(max(p[0], p[1]), max(p[2], p[3])), max(max(p[4], p[5]), max(p[6], p[7]))));*/
	//}
protected:
	Vec<D, T> m_min;
	Vec<D, T> m_max;
};

template<size_t M, size_t N, typename T = float> requires
(DATATYPE_FLOATING_Contract<T> || DATATYPE_INTEGER_Contract<T>) &&
DIMENSION_Contract<M> &&
DIMENSION_Contract<N>
class Mat
{
public:
	Mat() {}
	Mat(const Mat<M, N, T>& rhs) : m_data(rhs.m_data) {}
	Mat(Mat<M, N, T>&& rhs) : m_data(std::move(rhs.m_data)) {}
	template<size_t ... I>
	Mat(T* ptr, std::index_sequence<I...>) : m_data({((ptr+M*I), ...)}) {}
	Mat(T* ptr) : Mat(ptr, std::make_index_sequence<N>{}) {}
	template<size_t ... I>
	Mat(const T* const ptr, std::index_sequence<I...>) : m_data({((ptr+M*I), ...)}) {}
	Mat(const T* const ptr) : Mat(ptr, std::make_index_sequence<N>{}) {}
	template<typename ... Col> requires
	(std::is_same<Col, Vec<M, T>>::value && ...)
	Mat(Col&& ... cols) : m_data({(cols, ...)})
	{
		static_assert(sizeof ...(cols) == N);
	}
public:
	T* data() { return m_data[0].data(); }
	const T* const data() const { return m_data[0].data(); }
	Vec<M, T>& operator[](size_t i) { return m_data[i]; }
	const Vec<M, T>& operator[](size_t i) const { return m_data[i]; }
public:
	Mat<M, N, T> inv() const
	{
		throw std::logic_error("Not implemented");
		if constexpr (std::is_same<T, float>::value) {
		}
		else if constexpr (std::is_same<T, double>::value) {
		}
		else {
		}
	}
	template<int N2>
	Mat<M, N2, T> operator*(const Mat<N, N2, T>& rhs) const
	{
		Mat<M, N2, T> result;
		if constexpr (std::is_same<T, float>::value) {
			cblas_sgemm(CblasColMajor, CblasColMajor, M, N2, N, 1.0f, data(), 1, rhs.data(), 1, 0.0f, result.data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			cblas_dgemm(CblasColMajor, CblasColMajor, M, N2, N, 1.0, data(), 1, rhs.data(), 1, 0.0, result.data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return result;
	}
	Vec<M, T> operator*(const Vec<N, T>& rhs) const
	{
		Vec<M, T> result;
		if constexpr (std::is_same<T, float>::value) {
			cblas_sgemv(CblasColMajor, M, N, 1.0f, data(), N, rhs.data(), 1, 0.0f, result.data(), 1);
		}
		else if constexpr (std::is_same<T, double>::value) {
			cblas_dgemv(CblasColMajor, M, N, 1.0, data(), N, rhs.data(), 1, 0.0, result.data(), 1);
		}
		else {
			throw std::logic_error("Not implemented");
		}
		return result;
	}
protected:
	std::array<Vec<M, T>, N> m_data;
};
}

#endif
