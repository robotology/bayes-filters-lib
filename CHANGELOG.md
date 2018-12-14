# 📜 BayesFilters changelog

## Version 0.8.100

##### `Dependencies`
 - Removed OpenCV dependency.

##### `CMake`
 - Devel branch will now have +100 on the patch number to differentiate from master branch.
 - Fourth number of the project version has been removed to be compliant with SemVer system.
 - Third number of SemVer increases since API compatibility is broken.

#### `Data types`
 - Using only double data type within the whole library.

##### `Filtering Algorithms`
 - Added logging capabilities to FilteringAlgorithm.
 - Constructor SIS::SIS takes the state size, linear and circular, as argument (required to initialize ParticleSet).
 - Method SIS::filteringStep uses VectorXi instead of VectorXf to represent particle parents.
 - Method SIS::filteringStep uses particle weights in log space instead of linear space.
 - Moved default logging facilities of class SIS from method SIS::filteringStep to overriden method Logger::log.
 - Re-implemented class KalmanFilter, a general Gaussian filtering algorithm using a GaussianPrediction and a GaussianCorrection.
 - Renamed class KalmanFilter to GaussianFilter.
 - Added call to virtual method Logger::log in method GaussianFilter::filteringStep.

##### `Filtering Functions`
 - Removed VisualParticleFilter class.
 - Removed PFVisualCorrection and derived classes.
 - Added LikelihoodModel interface class.
 - Added GaussianLikelihood class.
 - Added class GaussianPrediction, a generic Gaussian prediction step.
 - Added class GaussianCorrection, a generic Gaussian correction step.
 - Added class KFPrediction, a (Gaussian) Kalman prediction step for LinearStateModel models.
 - Added class KFCorrection, a (Gaussian) Kalman correction step for LinearMeasurementModel models.
 - Added class UKFPrediction, a (Gaussian) unscented Kalman prediction step for StateModel and AdditiveStateModel models.
 - Added class UKFCorrection, a (Gaussian) unscented Kalman correction step for MeasurementModel and AdditiveMeasurementModel models.
 - Added class SUKFCorrection, a (Gaussian) unscented Kalman correction step for AdditiveMeasurementModel models.
 - Added class GPFPrediction, a particle filter prediction step that propagates the Gaussian belief associated to particles.
 - Added class GPFCorrection, a particle filter correction step that propagates the Gaussian belief associated to particles and weights them according to a Gaussian proposal distribution.
 - PFCorrection::getLikelihood() method is now pure virtual.
 - Used new ParticleSet class within classes PFPrediction, PFPredictionDecorator, PFCorrection, PFCorrectionDecorator, DrawParticles, UpdateParticles, Resampling, ResamplingWithPrior, ParticleSetInitialization, InitSurveillanceAreaGrid and SIS.
 - Method ResamplingWithPrior::resample heavily changed (due to use of ParticleSet).
 - Methods Resampling::resample and ResamplingWithPrior::resample use VectorXi instead of VectorXf to represent particle parents.
 - Methods Resampling::resample and Resampling::neff uses particle weights in log space instead of linear space.
 - Methods ResamplingWithPrior::resample uses particle weights in log space instead of linear space.
 - PFCorrection now performs measurements freeze before calling PFCorrection::correctStep and does not call it if measurements freeze fails.
 - UpdateParticles::correctStep now does not freeze measurements anymore.
 - UpdateParticles::correctStep uses particle weights in log space instead of linear space.
 - InitSurveillanceAreaGrid::initialize uses particle weights in log space instead of linear space.

###### State models
 - Added SimulatedStateModel class to simulate kinematic or dynamic models using StateModel classes.
 - Added non-pure virtual method StateModel::getTransitionProbability.
 - Added non-pure virtual method StateModel::getJacobian.
 - Added pure virtual method StateModel::getOutputSize
 - Added pure virtual method ExogenousModel::getOutputSize
 - Implemented method StateModelDecorator::getOutputSize
 - Implemented AdditiveStateModel class inheriting from StateModel.
 - Implemented LinearStateModel class inheriting from AdditiveStateModel.
 - Implemented LTIStateModel class inheriting from LinearStateModel.
 - WhiteNoiseAcceleration class now inherits from LinearStateModel.
 - Implemented method WhiteNoiseAcceleration::getOutputSize
 - Implemented method WhiteNoiseAcceleration::getTransitionProbability.

###### Measurement models
 - Added SimulatedLinearSensor class.
 - Added MeasurementModelDecorator class.
 - Added logging capabilities to MeasurementModel.
 - Removed method MeasurementModel::getNoiseSample.
 - Added pure virtual method MeasurementModel::getOutputSize
 - Method MeasurementModel::measure replaces method MeasurementModel::getAgentMeasurements and does not take the state as input.
 - Method MeasurementModel::freezeMeasurements replaces method MeasurementModel::bufferAgentData const.
 - Method UpdateParticles::correctStep uses MeasurementModel::freezeMeasurements.
 - Added class AdditiveMeasurementModel.
 - Added class LinearMeasurementModel.
 - Added class LTIMeasurementModel.
 - Renamed LinearSensor to LinearModel.
 - LinearModel class now inherits from LinearMeasurementModel.
 - LinearModel class now does not implement MeasurementModel::measure.

##### `Filtering Utilities`
 - Added Data class in order to have a type for encapsulating data coming from any process.
 - Added GaussianMixture, Gaussian and ParticleSet classes.
 - Added directional_add(), directional_sub() and directional_mean() functions in directional_statistics.h/cpp.
 - Added sigma_point(), unscented_weights() and unscented_transform() functions in sigma_point.h/cpp.
 - Added UTWeight struct to store unscented transform weights in sigma_point.h/cpp.
 - Added alias FunctionEvaluation in sigma_point.h/cpp.
 - Added method utils::log_sum_exp to evaluate the logarithm of a sum of exponentials.

##### `Test`
 - Added test_DirectionalStatisticsUtils for directional_statistics.h/cpp.
 - Added test_SigmaPointUtils for sigma_point.h/cpp.
 - Added test_Gaussian for Gaussian and GaussianMixture classes.
 - Added test_KF testing Gaussian filtering with KFPrediction and KFCorrection.
 - Added test_UKF testing Gaussian filtering with UKFPrediction and UKFCorrection.
 - Added test_mixed_KF_UKF testing Gaussian filtering with KFPrediction and UKFCorrection.
 - Added test_mixed_UKF_KF testing Gaussian filtering with UKFPrediction and KFCorrection.
 - Added test_mixed_KF_SUKF testing Gaussian filtering with KFPrediction and SUKFCorrection.
 - Added test_UPF testing particle filtering with UKFPrediction and UKFCorrection.
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
