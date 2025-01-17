# UAV Memory Isolation Experiment

This is an unmanned system scheduler and memory management developed by Keil 5.
We validated CoTMP and MINION in this scheduler.

## Directory Structure

1. **SYSTEM/mem**:
   - Implements memory allocation mechanisms.

2. **SYSTEM/rng**:
   - Provides a random number generation module.

3. **USER/AP_Scheduler.cpp**:
   - Contains the task scheduler from ArduPilot flight control system.

## Publication

Our paper **CToMP: a cycle-task-oriented memory protection scheme for unmanned systems** have been published in the journal **Science China Information Sciences**.

```
@article{:/publisher/Science China Press/journal/SCIENCE CHINA Information Sciences/67/6/10.1007/s11432-023-3865-0,
  author = "Chengyan MA,Ning XI,Di LU,Yebo FENG,Jianfeng MA",
  title = "CToMP: a cycle-task-oriented memory protection scheme for unmanned systems",
  journal = "SCIENCE CHINA Information Sciences",
  year = "2024",
  volume = "67",
  number = "6",
  pages = "162305-",
  url = "http://www.sciengine.com/publisher/Science China Press/journal/SCIENCE CHINA Information Sciences/67/6/10.1007/s11432-023-3865-0,
  doi = "https://doi.org/10.1007/s11432-023-3865-0"
}
```

