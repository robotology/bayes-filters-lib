# 📚 Bayes Filters Library

A flexible, modern, cross-platform C++ recursive Bayesian estimation library.

[![BayesFilters home](https://img.shields.io/badge/BayesFilters-Home%20%26%20Doc-E0C57F.svg?style=flat-square)](https://robotology.github.io/bayes-filters-lib/doc/html/index.html) [![Latest Release](https://img.shields.io/github/release/robotology/bayes-filters-lib.svg?style=flat-square&label=Latest%20Release)](https://github.com/robotology/bayes-filters-lib/releases) [![SemVer](https://img.shields.io/badge/SemVer-2.0.0-brightgreen.svg?style=flat-square)](http://semver.org/) [![ZenHub](https://img.shields.io/badge/Shipping_faster_with-ZenHub-blue.svg?style=flat-square)](https://zenhub.com)


# Overview
- [⚠️ About versioning](#️-about-versioning)
- [📖 Background](#-background)
- [🎛 Dependencies](#-dependencies)
- [🔨 Build and link the library](#-build-and-link-the-library)
- [🔬 Test the library](#-test-the-library)
- [📝 API documentation and example code](#-api-documentaion-and-example-code)
- [📑 Reference](#-reference)


# ⚠️ About versioning
The project is undergoing _heavy_ development: APIs will be subject to changes quite often.
To be able to understand API compatibility during development, the project will follow [SemVer](http://semver.org/) specs.

In particular, the library will have **zero major version**, i.e. **0.MINOR.PATCH**, as specified by [SemVer spec. 4](http://semver.org/#spec-item-4) and the project will comply with the following rules:
 1. **MINOR** version increases when API compatibility is broken;
 2. **PATCH** version increases when functionality are added in a backwards-compatible manner;
 3. Devel branch version adds 100 to **PATCH** version number, i.e. **0.MINOR.(PATCH+100)**.
 4. Additional labels for pre-release and build metadata are available as extensions to the 0.MINOR.PATCH format.


# 📖 Background
The main interest of the present library is estimation, which refers to inferring the values of a set of unknown variables from information provided by a set of noisy measurements whose values depend on such unknown variables.
Estimation theory dates back to the work of Gauss on determining the orbit of celestial bodies from their observations.
These studies led to the technique known as _Least Squares_.
Over centuries, many other techniques have been proposed in the field of estimation theory, e.g., the _Maximum Likelihood_, the _Maximum a Posteriori_ and the _Minimum Mean Square Error_ estimation.
The **Bayesian approach** models the quantities to be estimated as random variables characterized by Probability Density Functions (PDFs), and provides an improved estimation of such quantities by conditioning the PDFs on the available noisy measurements.
Recursive Bayesian estimation (or Bayesian filtering/filters) are a renowned and well-established probabilistic approach for recursively propagating, in a principled way via a two-step procedure, a PDF of a given time-dependent variable of interest.
Popular Bayes filters are the **Kalman** [1]-[4] and **particle filters** [5]-[7].

The aim of this library is to provide _interfaces_ and _implementations_ for new and existing recursive Bayesian filters!


# 🎛 Dependencies
Bayes Filters Library depends on
 - [Eigen3](https://bitbucket.org/eigen/eigen/) - `version >= 3.3 (no beta)`


# 🔨 Build and link the library
Use the following commands to build, install and link the library.

### Build
With `make` facilities:
```bash
$ git clone https://github.com/robotology/bayes-filters-lib
$ cd bayes-filters-lib
$ mkdir build && cd build
$ cmake ..
$ make
$ [sudo] make install
```

With `ninja` generator:
```bash
$ git clone https://github.com/robotology/bayes-filters-lib
$ cd bayes-filters-lib
$ mkdir build && cd build
$ cmake -GNinja ..
$ ninja
$ [sudo] ninja install
```

You can also generate IDE project (e.g. Visual Studio and Xcode) to use their
build tool facilities.

### Link
Once the library is installed, you can link it using `CMake` with as little effort as writing the following line of code in your project `CMakeLists.txt`:
```cmake
...
find_package(BayesFilters 0.MINOR.PATCH EXACT REQUIRED)
...
target_link_libraries(<target> BayesFilters::BayesFilters)
...
```


# 🔬 Test the library
We have designed some test to run with `CMake` to see whether everything run smoothly or not. Simply use
```cmake
$ ctest [-VV]
```
to run all the tests.

Tests are also a nice **starting points** to learn how to use the library and how to implement your own filters! _Just have a look at them!_


# 📝 API documentaion and example code
Doxygen-generated documentation is available [here](https://robotology.github.io/bayes-filters-lib/doc/html/index.html).


# 📑 Reference
[1] R. E. Kalman, “A new approach to linear filtering and prediction problems,” Trans. _Trans. ASME - Journal of Basic Engineering_, vol. 82 (Series D), no. 1, pp. 35– 45, 1960.  
[2] R. E. Kalman and R. S. Bucy, “New results in linear filtering and prediction theory,” _Trans. ASME - Journal of Basic Engineering_, vol. 83 (Series D), no. 1, pp. 95–108, 1961.  
[3] L. A. McGee, S. F. Schmidt and G. L. Smith, “Applications of statistical filter theory to the optimal estimation of position and velocity on board a circumlunar vehicle”, _NASA Technical Report R-135_, Tech. Rep., 1962.  
[4] S. J. Julier and J. K. Uhlmann, "Unscented filtering and nonlinear estimation", _Proceedings of the IEEE_, vol. 92, num. 3, pp. 401-422, 2004.  
[5] A. Doucet, N. De Freitas, N. Gordon, _Sequential Monte Carlo methods in practice_. Springer-Verlag, 2001.
[6] M. S. Arulampalam, S. Maskell, N. Gordon and T. Clapp, "A tutorial on particle filters for online nonlinear/non-Gaussian Bayesian tracking." _IEEE Transactions on signal processing_, vol. 50, num. 2, pp. 174-188, 2002.  
[7] N. Gordon, B. Ristic and S. Arulampalam. _Beyond the kalman filter: Particle filters for tracking applications_. Artech House, Boston, London, 2004.


---
[![how-to-export-cpp-library](https://img.shields.io/badge/-Project%20Template-brightgreen.svg?style=flat-square&logo=data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAAEAAAAA9CAYAAAAd1W%2FBAAAABmJLR0QA%2FwD%2FAP%2BgvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4QEFECsmoylg4QAABRdJREFUaN7tmmuIVVUUx%2F%2F7OmpaaGP6oedkGJWNIWoFVqRZGkIPSrAQgqhEqSYxszeFUB%2FCAqcXUaSRZmZP6IFm42QEUWAjqT1EQ0dLHTMfaWajv76sM%2BxO59znuY%2Bcs2CYmXv33mud31577bX3WU5lEEDOueDvfpLGSBolaaiksyUNknRyqNs%2BSR2SfrKf1ZJaJG11zv1rzJoX4ETgYWAtpcuvwCvABQHcJMUlPevAi5KmxTTbKalN0hZJ2yRlvO%2BOlzTYvOScmP5fSrreOber1mZcQF9gU2j2dgDNwLgixmwE7ge%2BC415FDi%2FFt1%2BuWfkRuBqH1CJYw8B3vfG7wR61NLDn%2BoZt6IcHma%2F7%2FX0zEo6HpRi4KWeYWOTNswfz9OzoKpr3ov2s4HNnmHtwMAy6Vvk6VkPjKkWgInA5zm2r0eBulJn3P6%2FEdgZo2c%2F8BDQu9wP3Qg8DRyIMGJPFhCfAjOAUcAgwOXQ08%2BC3hSb8SMF5AyfANcG4Iteip7L9QMejNjeAlkEjLZ1n490Ah023g%2FAZ0AL8DWwAdgO%2FBnT9y%2Fgdm8CllggbI9ouxeYD4wsNtBcBXwcY8hGYGqo7xjKJyuAyZ6uQ%2Fb5fO%2BzEcCbMf23ANNzeZ6AYcA8oxeWbcDcIAGJWKOlANgCfGNesBR4Cpjqz15ocgIAr0Z4bE%2FgDhsvSt71kzJAAm7O4uJvABfnSmhKBNBY4PL8D4CYdqcBc4CDETp%2Fs3g2SDFGNRVoVCkARhQYlwJ5vgD7JgDLInTvzsT0mQd8BFyTTzrrnGstd84hqR5Y5321LJtNHrABks6V1FfSkVCzeuUxQweAl4Ah2WAAd5XDA4AzgOdCfVbmAe4G22GI2SXATnGFyBrg1rikw05vhcpwIGMBrI%2Bt3UnAMxYgw7Lc7I7Sf7oF0ajcYZ%2BdTBuA24oF4O%2FnS4ErI4w4E3irgLF22f5%2FMEe7r4AJ3vG7y8WBO4Fvs0T%2B8SEb7y4VgC%2B%2FW0QdGFLSC5hmsaRYWWNp7ikRoK%2FL4uLrbZZ7xnhqFwBHske3lZKelfSBc%2B5o6G6wQdJIuxMcIKnBu5FykrZL2iVpq6TVzrm2CMMHS5ouaYak8MPtlfS6pGbn3Ibw3WQYgKTm8LaSpOwHFgCXJHAC7A80AW0xupb4SzGf%2BUx6CeSzxmcBmQLT8Yl2VoiSDZbx9SgSbkUB%2BPKeHZwyMSn1YOBJ4HBM9tYMnFfqNVs1AQTSYQ8zDOgN3AOsi2n7jn%2FxkUTIqgUAuWSTbW3lyi67ANSpdmS3pIWSXnbOra2U0loB8IikJ4JXYJWUTI0AaA%2F260q%2F%2F8uom0sKIAWQAkgBpABSACmAFEAKIAWQAkgBpABSACmAFEB3kc5uBSD0wuUySVN8AB3dgEF%2FK7PdLWmVpOCV3dGMpCGSZkr6%2FliabeA44CagVdIeSXMl1XtNV0kaH%2B58VkQ1RiXklgQBjAYWW11hVLXbfVY2k3OgKfZ%2BvuYB2Bvk2THltIetYOOiYl2pAXgM%2BLkWAHh21dkktcaM2WolgD3DgbCUCDoceK3KAC7MUkO8A5gJ1Fci2DQBP1YCAHCSFWD9EtH3b3Pxy6sVdYdaZVZHEgA8Fw%2Fi0BcxfVqAyUCvklw84STjCuDDEgEMBxbGtPsDeAA4odb34D5WZt%2BeJ4AmK6PZHPHdQeBtYOz%2FNTEZCbwQU%2FaSq0x%2BEtCnqi6eMIxJWUrZAxd%2FPHjoY%2FZQYrnFHIvqh2zNj6uGTf8ARTOPo64fR94AAAAASUVORK5CYII%3D)](https://github.com/robotology/how-to-export-cpp-library)
