# 📡 LoRa Indoor Localization Datasets

This repository hosts two publicly available datasets from the paper:  
**"Robust LoRa RSSI Fingerprinting with Inter-Gateway Fusion and Temporal Denoising."**

Both datasets were collected in a **13.5 m × 22.5 m indoor sports hall** using **LoRa SX1278 transceivers at 433 MHz**.  
Each grid coordinate represents a **25 cm × 25 cm tile**, enabling fine-grained indoor localization research.

---

## 🛰️ Overview

These datasets support indoor positioning research based on **LoRa RSSI fingerprinting**.  
They differ in whether **inter-gateway (receiver-to-receiver)** communication data is included.

| Dataset | Description | Data Type | Application |
|---|---|---|---|
| **Sensor-Only Dataset** | RSSI readings from the **sender to receivers only** | Sender → Receiver (S→G) | Baseline dataset for traditional fingerprinting |
| **Inter-Gateway Dataset** | Includes both **sender-to-receiver** and **receiver-to-receiver** RSSI links | Sender → Receiver + Receiver ↔ Receiver (S→G + G↔G) | Enhanced dataset for data fusion and filtering experiments |

---

## 📂 Dataset Descriptions

### 1) LoRa Indoor Localization — Sensor-Only Dataset

This dataset includes only RSSI values of messages sent from the **mobile sender node** to **four fixed receivers** positioned at the corners of the test environment.  
It excludes all inter-gateway (receiver-to-receiver) communication.

**File:** `LoRa_Indoor_Localization_SensorOnly.json`

**Columns**
```text
x, y, rssi
```

**Notes**
- `(x, y)` are tile indices on the 25 cm grid.
- `rssi` is RSSI in dBm as measured at a receiver from the sender.
- Multiple replicate readings may exist per coordinate (aggregate with mean/median if needed).

---

### 2) LoRa Indoor Localization — Inter-Gateway Dataset

This dataset extends Sensor-Only by adding **inter-gateway (receiver↔receiver)** RSSI links.  
Each receiver transmits to the other three receivers before the sender broadcasts, providing environmental context that improves robustness.

**File:** `LoRa_Indoor_Localization_InterGateway.xlsx`

**Columns**
```text
Comment,                             # coordinate as "x,y" (e.g., "27,3")
R1 to R2, R1 to R3, R1 to R4,        # inter-gateway (directed) links
R2 to R1, R2 to R3, R2 to R4,
R3 to R1, R3 to R2, R3 to R4,
R4 to R1, R4 to R2, R4 to R3,
S0 to R1, S0 to R2, S0 to R3, S0 to R4   # sender→receiver links
```

**Notes**
- Parse `Comment` into numeric `x, y`.
- RSSI values are in dBm (negative values).
- Use either **Sender-only features** (`S0 to R*`) or **Sender+IG** (all `* to *`) depending on your model.


---

## ⚙️ Experimental Setup

- **Environment:** 13.5 m × 22.5 m sports hall  
- **Grid resolution:** 0.25 m (25 cm per tile)  
- **Nodes:** 1 mobile sender (S0), 4 receivers (R1–R4) at corners  
- **Hardware:** ESP32-WROOM-32 + LoRa-02 SX1278 @ **433 MHz**  
- **LoRa parameters:** Bandwidth 62.5 kHz, SF7, CR 4/5, TX power 20 dBm  
- **Replicates:** both **3** and **5** per coordinate (short bursts / time series)

---

## 📬 Contact & License

- **Maintainer:** _Your Name_  
- **Institution:** Sharif University of Technology  
- **Email:** yourname@university.edu  
- **License:** MIT or CC-BY 4.0 (choose and update)
