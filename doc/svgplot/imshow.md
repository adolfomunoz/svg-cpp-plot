# GNUPlot::imsow

The `imshow` method provides a versatile way of plotting two dimensional data from the `svg_cpp_plot::SVGPlot` class. The simplest example is the following:

```cpp
svg_cpp_plot::SVGPlot plt;
plt.imshow({{0.0,0.1,0.2},
            {0.3,0.4,0.5},
			{0.6,0.7,0.8}});
plt.savefig("../doc/svgplot/imshow/example1.svg");
```

that generates:

<div style="text-align:center"><img 
 src="./imshow/example1.svg" alt="example1" width="50%" /></div>

