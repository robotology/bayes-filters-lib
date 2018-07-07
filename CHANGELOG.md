# 📜 BayesFilters changelog

## Version 0.7.101
##### `Dependencies`
 - Removed OpenCV dependency.

###### `CMake`
 - Devel branch will now have +100 on the patch number to differentiate from master branch.
 - Fourth number of the project version has been removed to be compliant with SemVer system.
 - Added a test for directionalstatisticsutils.h/cpp.
 - Added a test for sigmapointutils.h/cpp.
 - Added a test for Gaussian and GaussianMixture classes.

##### `Filtering classes`
 - Removed VisualParticleFilter class.
 - Removed PFVisualCorrection and derived classes.
 - Added GenericData class in order to have a type for encapsulating data coming from any process.
 - Added EigenMatrixData class that inherits from Generic data and Eigen::Matrix class.
 - Added EigenVectorXfData, EigenMatrixXfData, EigenMatrixXcfData, EigenVectorXdData, EigenMatrixXdData, EigenMatrixXcdData typedefs of EigenMatrixData.
 - MeasurementModel and direved classes now have registerProcessData() to register a GenericData coming from a process.
 - MeasurementModel and direved classes now have getProcessMeasurements() to provide meaurements of the registered GenericData of a process.
 - Added getter and setter methods to PFCorrection and derived classes for Process class.
 - Added Process interface class to model a generic processes.
 - Added SimulatedStateModel class to simulate kinematic or dynamic models using StateModel classes.
 - Added LikelihoodModel interface class to model generic likelihood models.
 - Added GaussianLikelihood class to model a likelihood for a linear process model with Gaussian distrubances.
 - Added Gaussian and GaussianMixture classes.
 - Added Particle and ParticleSet classes.
 - PFCorrection::getLikelihood() method is now pure virtual.

##### `Filtering Features`
 - Added directional_add(), directional_sub() and directional_mean() functions in directionalstatisticsutils.h/cpp.
 - Added unscented_weights() and unscented_transform() functions in sigmapointutils.h/cpp.
 - Added logging capabilities to FilteringAlgorithm, GenericData, MeasurementModel and Process interfaces.


## Version 0.7.1.0
##### `Bugfix`
 - Fixed WhiteNoiseAcceleration implementation.


## Version 0.7.0.0
##### `Filtering classes`
 - Removed PFVisualCorrection::getVisualObservationModel() and PFVisualCorrection::setVisualObservationModel().

##### `Bugfix`
 - Fixed PF prediction and correction move constructor (minor fix).

##### `CMake`
 - Threads is now a dependency of the exported library. This will solve a CMake error during configuration in CMake projects linking BayesFilters.
 - BayesFilters is now fully relocatable.


## Version 0.6.2.0
### Updates
##### `Filtering classes`
 - Added ResamplingWithPrior class.
 - Added HistoryBuffer and EstimatesExtraction classes.

##### `Filtering Features`
 - Added 'all' option to skip() method of particle filters. This option is particularly useful to reset the internal status of the filters.

##### `Dependencies`
 - Removed Eigen::NullaryExpr() calls. Now the library can be compiled with any Eigen 3.3 version.

##### `Bugfix`
 - Windows is now fully supported and tested.

##### `Test`
 - Added Appveyor and Travis yml files. Tests still need to be improved.

##### `CMake`
 - Updated CMake modules.


## Version 0.6.1.0
### Updates
##### `CMake`
 - Updated installation helper files, which are updated from YCM commit f162fcb.


## Version 0.6.0.1
### Bugfixes
 - `FilteringAlgorithm::filteringRecursion`: fix the invocation position of initialization.

## Version 0.6.0.0
 - This is the first public release of the BayesFilters library.
