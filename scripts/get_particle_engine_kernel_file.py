from sys import argv, stdout

with open(argv[1], 'r') as file:
    x_expression, y_expression = file.read().splitlines()

x_expression = x_expression.replace('x', 'vector.x').replace('y', 'vector.y')
y_expression = y_expression.replace('x', 'vector.x').replace('y', 'vector.y')

with open(argv[2], 'r') as kernel_file:
    kernel_file_contents = kernel_file.read()

    kernel_file_contents = kernel_file_contents.replace('@x_expression', x_expression)
    kernel_file_contents = kernel_file_contents.replace('@y_expression', y_expression)

stdout.write(kernel_file_contents)
