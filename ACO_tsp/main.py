import argparse
from Visual_and_tools import initial
from ACO import ACO

def default_argument_parser():
    paser = argparse.ArgumentParser(description="Ant Colony Algorithm")
    paser.add_argument("--ant_num",default=5,type=int)
    paser.add_argument("--dot_num",default=10,type=int)
    paser.add_argument("--iteration",default=50,type=int)
    paser.add_argument("--alpha",default=2.0,type=float)
    paser.add_argument("--beta", default=3.0, type=float)
    paser.add_argument("--rho",default=0.6,type=float) # 0-1
    paser.add_argument("--q",default=100,type=float)

    return  paser

def main():
    args = default_argument_parser().parse_args()
    dots, distance, pheromone = initial(args.dot_num)

    aco = ACO(
        ant_num=args.ant_num, iteration = args.iteration,
        alpha=args.alpha, beta=args.beta, rho=args.rho, q=args.q,
        dots=dots,distance=distance,pheromone=pheromone
    )
    aco.run()

if __name__ == '__main__':
    main()