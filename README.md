Given a vector $a$ containing arbitrary functions of $x$ and $y$ and another vector $b$;

$$\overrightarrow{a}\left ( x, y \right ) = \begin{bmatrix} f\left ( x, y \right )\\
g\left ( x, y \right ) \end{bmatrix}$$

$$\overrightarrow{b} = \begin{bmatrix}x\\
y\end{bmatrix} + \overrightarrow{a}\left ( x, y \right )$$

$b$ is normalised and plotted at fixed intervals on $x$ and $y$. The functions $f$ and $g$ can be any function which will be used to determine the behaviour of the vector field in the x- and y-directions, respectively. Vector magnitude is shown by a red to blue colour gradient, representing the highest and lowest magnitudes, respectively within the current viewport. The positions of the particles are computed using the GPU through OpenCL to reduce load on the CPU allowing for the rendering of many more particles.

| Control | Function |
| --- | --- |
| Mouse drag | Move the coordinate axes; change the viewport of the coordinate system |
| Mouse wheel | Change the zoom level of the coordinate system |
| A key | Toggle visibility of the axes |
| Space key | Reset position of the axes |

In the demonstrations below, the chosen functions are the equations of motion of a pendulum where we rename the x - y plane as the $\theta$ - $v$ plane representing the angular position and velocity, respectively, of a pendulum under gravity.

$$\begin{bmatrix} f\left ( x, y \right )\\
g\left ( x, y \right ) \end{bmatrix} = \begin{bmatrix} f\left ( \theta, v \right )\\
g\left ( \theta, v \right ) \end{bmatrix} = \begin{bmatrix} v\\
-\mu v - \frac{g}{L} sin \left ( \theta \right ) \end{bmatrix}$$

| ![demo](https://github.com/maskrosbarn/opencl-vector-field/assets/91302084/a8665cc3-581d-4fb3-82dc-21dd2abf59bf) | ![pan_demo](https://github.com/maskrosbarn/opencl-vector-field/assets/91302084/e96a9c5f-cba0-4f71-b23e-f456fe3844cf) | ![zoom_demo](https://github.com/maskrosbarn/opencl-vector-field/assets/91302084/3df156e7-ec98-4c9f-8e42-1a8b33beb19f) |
| --- | --- | --- |
| Particle Motion | Panning | Zooming |
