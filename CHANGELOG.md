# 📜 BayesFilters changelog

## Version 0.8.100
##### `Dependencies`
 - Removed OpenCV dependency.

##### `CMake`
 - Devel branch will now have +100 on the patch number to differentiate from master branch.
 - Fourth number of the project version has been removed to be compliant with SemVer system.
 - Third number of SemVer increases since API compatibility is broken.

##### `Filtering Algorithms`
 - Constructor SIS::SIS takes the state size as argument (required to initialize ParticleSet).
 - Method SIS::filteringStep uses VectorXi instead of VectorXf to represent particle parents.

##### `Filtering Functions`
 - Removed VisualParticleFilter class.
 - Removed PFVisualCorrection and derived classes.
 - Added getter and setter methods to PFCorrection and derived classes for Process class.
 - Added Process interface class to model a generic processes.
 - Added LikelihoodModel interface class to model generic likelihood models.
 - Added GaussianLikelihood class to model a likelihood for a linear process model with Gaussian distrubances.
 - PFCorrection::getLikelihood() method is now pure virtual.
 - Used ParticleSet class within classes PFPrediction, PFPredictionDecorator, PFCorrection, PFCorrectionDecorator, DrawParticles, UpdateParticles, Resampling, ResamplingWithPrior, ParticleSetInitialization, InitSurveillanceAreaGrid and SIS.
 - Method ResamplingWithPrior::resample heavily changed (due to use of ParticleSet).
 - Methods Resampling::resample and ResamplingWithPrior::resample use VectorXi instead of VectorXf to represent particle parents.

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
 - SimulatedStateModel class now supports state vector of any size.

###### Measurement models
 - MeasurementModel and direved classes now have registerProcessData() to register a GenericData coming from a process.
 - MeasurementModel and direved classes now have getProcessMeasurements() to provide meaurements of the registered GenericData of a process.
 - Removed method MeasurementModel::getNoiseSample.
 - Removed method MeasurementModelDecorator::getNoiseSample.
 - Added class LinearMeasurementModel.
 - Added class LTIMeasurementModel.
 - LinearModel class now inherits from LinearMeasurementModel.
 - Method MeasurementModel::measure replaces method MeasurementModel::getAgentMeasurements and does not take the state as input.
 - Method MeasurementModelDecorator::measure replaces method MeasurementModelDecorator::getAgentMesurements.
 - Method SimulatedLinearSensor::measure replaces method SimulatedLinearSensor::getAgentMeasurements.
 - LinearModel class now does not implement MeasurementModel::measure.
 - SimulatedLinearModel class inheriting from LinearModel implements method MeasurementModel::measure.
 - Method GaussianLilkelihood::likelihood uses method MeasurementModel::measure.
 - Method MeasurementModel::freezeMeasurements replaces method MeasurementModel::bufferAgentData const.
 - Method MeasurementModelDecorator::freezeMeasurements replaces method MeasurementModelDecorator::bufferAgentData const.
 - Method SimulatedLinearSensor::freezeMeasurements replaces method SimulatedLinearSensors::bufferAgentData const.
 - Moved actual evaluation of measurements from SimulatedLinearSensor::measure to SimulatedLinearSensors::freezeMeasurements.
 - Method UpdateParticles::correctStep uses MeasurementModel::freezeMeasurements.

##### `Filtering Utilities`
 - Added Particle and ParticleSet classes.
 - Added GenericData class in order to have a type for encapsulating data coming from any process.
 - Added EigenMatrixData class that inherits from Generic data and Eigen::Matrix class.
 - Added EigenVectorXfData, EigenMatrixXfData, EigenMatrixXcfData, EigenVectorXdData, EigenMatrixXdData, EigenMatrixXcdData typedefs of EigenMatrixData.
 - Added Gaussian and GaussianMixture classes.
 - Added directional_add(), directional_sub() and directional_mean() functions in directionalstatisticsutils.h/cpp.
 - Added unscented_weights() and unscented_transform() functions in sigmapointutils.h/cpp.
 - Added logging capabilities to FilteringAlgorithm, GenericData, MeasurementModel and Process interfaces.
 - Removed GaussianRef class.
 - Added methods to get the i-th mean, covariance and weight of a GaussianMixture.
 - Added accessors to single elements of the mean/covariance in GaussianMixture class.
 - Re-implemented Gaussian class as inheriting from GaussianMixture class.
 - Added accessors to single elements of the mean/covariance in Gaussian class.
 - Removed Particle class.
 - Implemented ParticleSet class as inheriring from GaussianMixture class.

##### `Test`
 - Added a test for directionalstatisticsutils.h/cpp.
 - Added a test for sigmapointutils.h/cpp.
 - Added a test for Gaussian and GaussianMixture classes.
 - Updated test_SIS_Decorators (since use MeasurementModelDecorator).
 - Updated test_Gaussian (since use Gaussian class).
 - Fix typo in test_Gaussian (since returning in case of failure outside the catch block).
 - Updated test_SigmaPointUtils (since use Gaussian class).
 - Updated test_SIS (since use SIS class).
 - Updated test_SIS_Decorators (since use classes PFPredictionDecorator, PFCorrectionDecorator and SIS)


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
