#include<stdio.h>
#include<mpi.h>

double func(double x){
  return x*x;
}

double Trap(double a, double b, double h){
  double sum=0.0;
  
  for(double i=a+h;i<=b-h;i+=h){
   sum+=func(i);
  }
  sum+=(func(a)+func(b))/2;
  return sum*h;

}
int main(){

  int rank, size;
  double a, b, h, n;
  double n_p, a_p, b_p, c_p, sum_p, final_sum=0.0;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if(rank==0){
    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%lf", &n);
    
    n_p = n/size;

    h = (b-a)/n;
  }
  MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&h, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a_p = a+(rank*h*n_p);
  b_p = a_p+h*n_p;
  
  sum_p = Trap(a_p, b_p, h);
  if(rank!=0)
    MPI_Send(&sum_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
 
  printf("%d process, sum %lf\n", rank, sum_p);
  if(rank==0){
    final_sum+=sum_p;
    for(int tr=1;tr<size;tr++){
      MPI_Recv(&sum_p, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      final_sum+=sum_p;
    }
    printf("The area under the curve is %lf", final_sum);
  }
  MPI_Finalize();
  return 0;
}
