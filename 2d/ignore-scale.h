namespace svg_cpp_plot {
namespace _2d {

class ignore_scale : public _2d::Element {
	std::shared_ptr<_2d::Element> unscaled;

public:
	ignore_scale(const std::shared_ptr<_2d::Element>& u) : unscaled(u) {}
/*	ignore_scale(std::shared_ptr<_2d::Element>&& u) : unscaled(u) {}
    template<typename T>
    ignore_scale(const T& t) : unscaled(std::make_shared<T>(t)) {}
    template<typename T>
    ignore_scale(T&& t) : unscaled(std::make_shared<T>(std::forward<T>(t))) {}
*/	std::string to_string(const Matrix& m) const noexcept override {
        return unscaled->to_string(translate(m[0][2],m[1][2]));
	}
    _2d::Element& ref() { return *unscaled; }
};

}
}