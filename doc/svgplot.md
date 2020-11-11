# SVGPlot

Plotting with svg-cpp-plot is designed to be similar to (or more accurately, a subset of) `matplotlib.pyplot` or `MATLAB` in terms of plotting. Each method makes some change to a figure: e.g., creates a figure, creates a plotting area in a figure, plots some lines in a plotting area, decorates the plot with labels, etc. There is, however, a small difference: all the functions are translated into methods of the class `svg_cpp_plot::SVGPlot`. Each of these methods provides a functionality similar to their `matplotlib.pyplot` counterparts:

- [`plot`](./svgplot/plot.md): plots *x,y* scatter plots or lines with different styles and configuration options. 
- [`imshow`](./svgplot/imshow.md): plots two-dimensional data.

Finally, the method `savefig("file.svg")` enables the saving of the corresponding plot into the *file.svg* file. The format of this svg is designed so that it holds reasonably well when converting it to pdf, for instance with the vector drawing software [Inkscape](https://inkscape.org/). In fact, with Inkscape it is possible to convert svg to pdf using the command line:

```
inkscape file.svg --export-pdf=file.pdf
```

