# 2. WRO 2026 Requirements Map

This document maps the repository to the WRO 2026 Future Engineers documentation expectations.

## Repository Requirements

| Requirement | Repository Response | Status |
| --- | --- | --- |
| Public GitHub repository | Public repo: `Yamilgmr/SKRobotics_WRO2026` | Done |
| README in English with at least 5000 characters | Main README is written in English and exceeds the minimum target | Done |
| Source code for programmed components | Current Open Challenge Arduino Mega code plus obstacle placeholder in `src/` | Open Challenge current-final, obstacle pending |
| Vehicle photos | Six final current vehicle views are stored in `v-photos/` | Done |
| Team photos | Formal, individual, and working-session photos are stored in `t-photos/` | Done |
| Demonstration videos | Link table in `video/video.md` with current Open Challenge YouTube Short | Link added, verify 30-second autonomous-driving requirement |
| Wiring and electromechanical diagrams | SVG wiring diagram and Markdown diagrams in `schemes/` | Diagram added, physical verification pending |
| Calibration and test data | Serial Monitor test evidence is stored in `assets/test_evidence/`; numeric calibration tables are in `data/calibration/` | Initial evidence added, measured tables pending |
| CAD or fabrication files | Placeholder structure in `models/` | Pending real CAD |
| Engineering process evidence | Decision log, test templates, and real Serial Monitor evidence | Started |
| At least three meaningful commits | Repository history contains multiple meaningful commits | Done |

## Rubric Alignment

### Criterion 1: Mobility and Mechanical Design

Evidence location:

- `docs/03_mechanical_mobility.md`
- `models/`
- `v-photos/`
- `engineering-journal/decision-log.md`

Target evidence:

- Steering geometry.
- Drive motor selection and torque/speed reasoning.
- Wheelbase and track width.
- Servo limits.
- Mechanical trade-offs.
- Test results for turn radius and stability.

### Criterion 2: Power and Sensor Architecture

Evidence location:

- `docs/04_power_and_sensors.md`
- `schemes/power_architecture.md`
- `schemes/electromechanical_overview.md`
- `data/calibration/`

Target evidence:

- Battery voltage and current budget.
- Motor driver selection and reason.
- Sensor placement geometry.
- Noise and failure handling.
- Calibration method.

### Criterion 3: Software Architecture and Obstacle Strategy

Evidence location:

- `docs/05_software_architecture.md`
- `docs/06_open_challenge_strategy.md`
- `docs/07_obstacle_challenge_strategy.md`
- `src/`

Target evidence:

- State machine.
- Algorithms and constants.
- Lane following strategy.
- Obstacle behavior.
- Edge cases.
- Tuning metrics.

### Criterion 4: System Thinking and Engineering Decisions

Evidence location:

- `docs/09_engineering_decisions.md`
- `docs/11_risk_register.md`
- `engineering-journal/decision-log.md`

Target evidence:

- Constraints.
- Trade-offs.
- Iterations.
- Risk mitigation.
- Data-based "why we chose X" reasoning.

### Criterion 5: Reproducibility and GitHub Quality

Evidence location:

- `README.md`
- `CHANGELOG.md`
- `src/README.md`
- all required WRO folders

Target evidence:

- Clear structure.
- Meaningful commit history.
- Build/upload instructions.
- Wiring, code, CAD, and photos.
- Test workflow and release notes.

