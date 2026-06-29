# Lecture 5: FSMs and Timing Analysis
* **State Machines**:
    * **Moore**: Outputs are a function only of the current State Register.
    * **Mealy**: Outputs are a function of current State AND current Inputs.
* **Timing Constraints**:
    * **Setup Time ({su}$)**: Minimum time data must be stable *before* the clock edge.
    * **Hold Time ($)**: Minimum time data must be stable *after* the clock edge to prevent metastability.
