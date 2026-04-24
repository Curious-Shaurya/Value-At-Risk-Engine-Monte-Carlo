# Value-At-Risk-Engine-Monte-Carlo
This is a high-performance Monte Carlo Simulation Engine written in C++. It is designed to estimate Value at Risk (VaR) for financial portfolios by simulating hundreds of thousands of potential market scenarios. By utilising random sampling and statistical distributions, the engine provides a probabilistic view of potential losses, helping to quantify risk in volatile environments.

$S_{t+\Delta t} = S_t \times \exp\left(\left(\mu - \frac{\sigma^2}{2}\right)\Delta t + \sigma \sqrt{\Delta t} \cdot Z\right)$

