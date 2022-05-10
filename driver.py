import subprocess
from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt

makefile = "mem-chase"


def run_instance(num, clean):
    
    correct = subprocess.run(["./"+makefile, str(num), str(clean)],universal_newlines = True, stdout=subprocess.PIPE)
    output = correct.stdout.splitlines()
    #print(output)
    time = output[-1].split("=")[-1].strip()
    #print(time)
    time = float(time)
    #print(time)
    return time

def main():
    subprocess.run(["make","clean",makefile])
    fast = []
    slow = []
    for i in range(9,20):
        print("starting with "+ str(i))
        slow_val = run_instance(i, 1)
        slow.append(slow_val)
        fast_val =run_instance(i,0)
        fast.append(fast_val)
    print(slow)
    print(fast)
    x = [i for i in range(9,27)]
    z = [fast[i]/slow[i] for i in range(len(slow))]
    y = slow + fast
    x+=x
    z = z + z

    #write this to csv file b/c can't do fig here
    f = open("output.txt", 'w')
    if (True):
        for x_ in x:
            f.write(str(x_) + ",")
        f.write("\n")
        for y_ in y:
            f.write(str(y_) +",")
        f.write("\n")
        for z_ in z:
            f.write(str(z_) +",")
    f.close
        
    """
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.plot(xs = x, ys = y, zs = z)
    #plt.show()
    plt.savefig("graph1.png")
    """
main()