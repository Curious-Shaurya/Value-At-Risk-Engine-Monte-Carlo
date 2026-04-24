# Value-At-Risk-Engine-Monte-Carlo
### Overview
This is a high-performance Monte Carlo Simulation Engine written in C++. It is designed to estimate Value at Risk (VaR) for financial portfolios by simulating hundreds of thousands of potential market scenarios. By utilising random sampling and statistical distributions, the engine provides a probabilistic view of potential losses, helping to quantify risk in volatile environments.

### The Mathematical Model
$S_{t+\Delta t} = S_t \times \exp\left(\left(\mu - \frac{\sigma^2}{2}\right)\Delta t + \sigma \sqrt{\Delta t} \cdot Z\right)$

Where: 
S_t is the current price 
\mu is the drift (annual expected return)
\sigma is the volatility (annual standard deviation) 
\Delta t is the change in time (1/252 for a trading year) 
Z is the correlated random shock everyday. 

### The Performance Metrics
The following tests were conducted on my local machine using a multi-threaded C++ execution across 8 cores.

| Number of Simulations | Execution Time (ms) | Execution Time (seconds) |
| :--- | :--- | :--- |
| 10,000 | 376 ms | 0.38 s |
| 100,000 | 3,360 ms | 3.36 s |
| 500,000 | 15,641 ms | 15.64 s |
| 1,000,000 | 31,096 ms | 31.10 s |
