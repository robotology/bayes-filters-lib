# 📚 Bayes Filters Library {#mainpage}

# Overview {#overview}

A flexible, modern, cross-platform C++ recursive Bayesian estimation library.
[TOC]


# ⚠️ About versioning {#versioning}

---
The project is undergoing _heavy_ development: APIs will be subject to changes quite often.
To be able to understand API compatibility during development, the project will follow [SemVer](http://semver.org/) specs.

In particular, the library will have **zero major version**, i.e. **0.MINOR.PATCH**, as specified by [SemVer spec. 4](http://semver.org/#spec-item-4) and the project will comply with the following rules:
 1. **MINOR** version increases when API compatibility is broken;
 2. **PATCH** version increases when functionality are added in a backwards-compatible manner;
 3. Additional labels for pre-release and build metadata are available as extensions to the 0.MINOR.PATCH format.


# 📖 Background {#background}

---
The main interest of the present library is estimation, which refers to inferring the values of a set of unknown variables from information provided by a set of noisy measurements whose values depend on such unknown variables.
Estimation theory dates back to the work of Gauss on determining the orbit of celestial bodies from their observations.
These studies led to the technique known as _Least Squares_.
Over centuries, many other techniques have been proposed in the field of estimation theory, e.g., the _Maximum Likelihood_, the _Maximum a Posteriori_ and the _Minimum Mean Square Error_ estimation.
The **Bayesian approach** models the quantities to be estimated as random variables characterized by Probability Density Functions (PDFs), and provides an improved estimation of such quantities by conditioning the PDFs on the available noisy measurements.
Recursive Bayesian estimation (or Bayesian filtering/filters) are a renowned and well-established probabilistic approach for recursively propagating, in a principled way via a two-step procedure, a PDF of a given time-dependent variable of interest.
Popular Bayes filters are the **Kalman** [1]-[4] and **Particle** filters [5]-[7].

The aim of this library is to provide _interfaces_ to implement new Bayes filters as well as _providing implementation_ of existing filters.


# 🎛 Dependencies {#dependencies}

---
Bayes Filters Library depends on
 - [Eigen3](https://bitbucket.org/eigen/eigen/) - `version >= 3.3 (no beta)`


# 🔨 Build and link the library {#build-and-link-the-library}

---

Use the following commands to build, install and link the library.


## Build {#build}
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

## Link {#link}
Once the library is installed, you can link it using `CMake` with as little effort as writing the following line of code in your project `CMakeLists.txt`:
```cmake
...
find_package(BayesFilters 0.MINOR.PATCH EXACT REQUIRED)
...
target_link_libraries(<target> BayesFilters::BayesFilters)
...
```


# 🔬 Test the library {#test-the-library}

---
We have designed some test to run with `CMake` to see whether everything run smoothly or not. Simply use
```cmake
$ ctest [-VV]
```
to run all the tests.

Tests are also a nice **starting points** to learn how to use the library and how to implement your own filters! _Just have a look at them!_


# 📘 Tutorials {#tutorials}

---
The best way to learn the basic principles about the library is by examples:
 - [The first Kalman filter](@ref kf)
 - [Another Kalman-like filter: the Unscented Kalman Filter](@ref ukf)
 - [The first particle filter: the Sequential Importance Sampling PF](@ref sis)
 - [The best from particle and Kalman filtering: the Unscented Particle Filter](@ref upf)


# 📑 Reference {#reference}

---
[1] R. E. Kalman, “A new approach to linear filtering and prediction problems,” Trans. _Trans. ASME - Journal of Basic Engineering_, vol. 82 (Series D), no. 1, pp. 35– 45, 1960.<br>
[2] R. E. Kalman and R. S. Bucy, “New results in linear filtering and prediction theory,” _Trans. ASME - Journal of Basic Engineering_, vol. 83 (Series D), no. 1, pp. 95–108, 1961.<br>
[3] L. A. McGee, S. F. Schmidt and G. L. Smith, “Applications of statistical filter theory to the optimal estimation of position and velocity on board a circumlunar vehicle”, _NASA Technical Report R-135_, Tech. Rep., 1962.<br>
[4] S. J. Julier and J. K. Uhlmann, "Unscented filtering and nonlinear estimation", _Proceedings of the IEEE_, vol. 92, num. 3, pp. 401-422, 2004.<br>
[5] A. Doucet, N. De Freitas, N. Gordon, _Sequential Monte Carlo methods in practice_. Springer-Verlag, 2001.<br>
[6] M. S. Arulampalam, S. Maskell, N. Gordon and T. Clapp, "A tutorial on particle filters for online nonlinear/non-Gaussian Bayesian tracking." _IEEE Transactions on signal processing_, vol. 50, num. 2, pp. 174-188, 2002.<br>
[7] N. Gordon, B. Ristic and S. Arulampalam. _Beyond the kalman filter: Particle filters for tracking applications_. Artech House, Boston, London, 2004.<br>
