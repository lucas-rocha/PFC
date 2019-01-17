% Simple test for our coordinate descent methods on a randomly generated
% matrix. 
% 
% See A. Vandaele, N. Gillis, Q. Lei, K. Zhong, I. Dhillon, Coordinate 
% Descent Methods for Symmetric Nonnegative Matrix Factorization, arXiv, 
% 2015. 

% Randomly generated symmetric nonnegative matrix A to factorize
%n=500; A=rand(n,n);
%A=A+A';
files = dir('../Correlacao Cosseno/*');

for file = files'
    disp('-------------------------------')
    disp(file.name)
    arquivo = strcat('../Correlacao Cosseno/', file.name)
    A = dlmread(arquivo);

    % Inner rank of the factorization
    r=10;

    % Options (see loadoptions.m)
    options.maxiter   = 100;
    options.timelimit = 5;
    options.initmatrix='dense01'; % random initialization
    options.seed=0; % Use same seed to have the same initialization 

    % Comparison of two CD methods: cyclic updates of the variables vs. 
    % random shuffling of the columns. 
    options.shuffle_columns = 0;
    tic; [H1,e1,t1] = symNMF(A,r,options); toc

    %B = H1 * H1';
    %disp(B)
    
    arquivo = strcat('../U/', file.name)
    dlmwrite(arquivo, H1, ' ')
  
end
