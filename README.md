Given a vector $a$ containing arbitrary functions of $x$ and $y$ and another vector $b$;

$$\overrightarrow{a}\left ( x, y \right ) = \begin{bmatrix} f\left ( x, y \right )\\
g\left ( x, y \right ) \end{bmatrix}$$

$$\overrightarrow{b} = \begin{bmatrix}x\\
y\end{bmatrix} + \overrightarrow{a}\left ( x, y \right )$$

$b$ is normalised and plotted at fixed intervals on $x$ and $y$. The functions $f$ and $g$ can be any function which will be used to determine the behaviour of the vector field in the x- and y-directions, respectively. Vector magnitude is shown by a red to blue colour gradient, representing the highest and lowest magnitudes, respectively within the current viewport.

| Control | Function |
| --- | --- |
| Mouse drag | Move the coordinate axes; change the viewport of the coordinate system |
| Mouse wheel | Change the zoom level of the coordinate system |
| A key | Toggle visibility of the axes |
| Space key | Reset position of the axes |

In the demonstration below, the chosen functions are the equations of motion of a pendulum where we rename the x - y plane as the $\theta$ - $v$ plane representing the angular position and velocity, respectively, of a pendulum under gravity.

$$\begin{bmatrix} f\left ( x, y \right )\\
g\left ( x, y \right ) \end{bmatrix} = \begin{bmatrix} f\left ( \theta, v \right )\\
g\left ( \theta, v \right ) \end{bmatrix} = \begin{bmatrix} v\\
-\mu v - \frac{g}{L} sin \left ( \theta \right ) \end{bmatrix}$$
