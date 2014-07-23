SpectralProfiling
=================
The aim of this project is to develop an accurate recommender system based
upon the spectral representation of users. The user-item data that is to be
employed throughout this project is provided from the Netflix Prize dataset
and is primarily structured in the form of a large scale, incomplete matrix.
The matrix is thus reformulated using Singular Value Decomposition in order 
to approximate a best fit subspace within each dimension (column) of the
matrix. Following this process, user information will be embedded within
a predefined low-dimensional space, profile space, and described by means
of a profile vector. Users with similar profile vectors will be situated within
close vicinity of one another within the profile space. Finally, by using this
characteristic, users can then be classified into partitioned classes according
to their statistical similarities. There will be a step-by-step mathematical
description of how these complex algorithms are developed, and how they
are eventually incorporated within a large scale User-Item Recommendation
model. In order to evaluate the efficiency of each of these corresponding al-
gorithms, the predictions formulated by the Recommender System will be
benchmarked by a Test Set that consists of a known set of dataset values.
The collection of algorithms that will be discussed within this paper in-
clude: Spectral Profiling of Probabilistic Taste, Singular Value Decomposi-
tion, k-dimensional tree data structures and Time changing Baseline Pre-
dictor Modelling.
