# 📜 BayesFilters changelog

## Version 0.8.100

##### `Dependencies`
 - Removed OpenCV dependency.

##### `CMake`
 - Devel branch will now have +100 on the patch number to differentiate from master branch.
 - Fourth number of the project version has been removed to be compliant with SemVer system.
 - Third number of SemVer increases since API compatibility is broken.

##### `Filtering Algorithms`
 - Added logging capabilities to FilteringAlgorithm.
 - Constructor SIS::SIS takes the state size as argument (required to initialize ParticleSet).
 - Method SIS::filteringStep uses VectorXi instead of VectorXf to represent particle parents.

##### `Filtering Functions`
 - Removed VisualParticleFilter class.
 - Removed PFVisualCorrection and derived classes.
 - Added LikelihoodModel interface class.
 - Added GaussianLikelihood class.
 - PFCorrection::getLikelihood() method is now pure virtual.
 - Used new ParticleSet class within classes PFPrediction, PFPredictionDecorator, PFCorrection, PFCorrectionDecorator, DrawParticles, UpdateParticles, Resampling, ResamplingWithPrior, ParticleSetInitialization, InitSurveillanceAreaGrid and SIS.
 - Method ResamplingWithPrior::resample heavily changed (due to use of ParticleSet).
 - Methods Resampling::resample and ResamplingWithPrior::resample use VectorXi instead of VectorXf to represent particle parents.
 - PFCorrection now performs measurements freeze before calling PFCorrection::correctStep and does not call it if measurements freeze fails.
 - UpdateParticles::correctStep now does not freeze measurements anymore.

###### State models
 - Added SimulatedStateModel class to simulate kinematic or dynamic models using StateModel classes.
 - Removed method StateModel::getNoiseCovarianceMatrix.
 - Removed method StateModel::getNoiseSample.
 - Removed method StateModelDecorator::getNoiseCovarianceMatrix.
 - Removed method StateModelDecorator::getNoiseSample.
 - Added non-pure virtual method StateModel::getTransitionProbability.
 - Added non-pure virtual method StateModel::getJacobian.
 - Implemented AdditiveStateModel class inheriting from StateModel.
 - Implemented LinearStateModel class inheriting from AdditiveStateModel.
 - Implemented LTIStateModel class inheriting from LinearStateModel.
 - WhiteNoiseAcceleration class now inherits from LinearStateModel.

###### Measurement models
 - Added SimulatedLinearSensor class.
 - Added MeasurementModelDecorator class.
 - Added logging capabilities to MeasurementModel.
 - Removed method MeasurementModel::getNoiseSample.
 - Method MeasurementModel::measure replaces method MeasurementModel::getAgentMeasurements and does not take the state as input.
 - Method MeasurementModel::freezeMeasurements replaces method MeasurementModel::bufferAgentData const.
 - Method UpdateParticles::correctStep uses MeasurementModel::freezeMeasurements.
 - Added class LinearMeasurementModel.
 - Added class LTIMeasurementModel.
 - Renamed LinearSensor to LinearModel.
 - LinearModel class now inherits from LinearMeasurementModel.
 - LinearModel class now does not implement MeasurementModel::measure.

##### `Filtering Utilities`
 - Added Data class in order to have a type for encapsulating data coming from any process.
 - Added GaussianMixture, Gaussian and ParticleSet classes.
 - Added directional_add(), directional_sub() and directional_mean() functions in directional_statistics.h/cpp.
 - Added unscented_weights() and unscented_transform() functions in sigma_point.h/cpp.

##### `Test`
 - Added test_DirectionalStatisticsUtils for directional_statistics.h/cpp.
 - Added test_SigmaPointUtils for sigma_point.h/cpp.
 - Added test_Gaussian for Gaussian and GaussianMixture classes.
 - Updated test_SIS.
 - Updated test_SIS_Decorators.


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
