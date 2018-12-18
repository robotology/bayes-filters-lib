# How to decorate classes to add functionalities {#decorate-classes}

The [_Decroator patter_](https://en.wikipedia.org/wiki/Decorator_pattern) is
extensively used within the library to dynamically add new functionality to other classes.
This is particularly useful to design flexible and reusable object-oriented software
providing an alternative to subclassing.
In particular, when there are **several independent ways of extending functionality**
and it is difficult to predict, at design time, what combinations of such
functionalities will be needed, subclassing would require to make every possible
(or most of the) combination of such functionalities, while **decorators provide
much more flexibility combining functionalities on a per-use basis and foster
code reuse**.<br>

The following snippet code shows how to decorate filtering classes like
[StateModel](@ref bfl::StateModel), [ObservationModel](@ref bfl::ObservationModel),
[PFPrediction](@ref bfl::PFPrediction) and [PFCorrection](@ref bfl::PFCorrection).

\includelineno test_SIS_Decorators/main.cpp
