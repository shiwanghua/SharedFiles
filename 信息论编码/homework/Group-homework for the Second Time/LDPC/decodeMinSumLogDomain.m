function vHat = decodeMinSumLogDomain(rx, H, N0, iteration)
% Log-domain sum product algorithm LDPC decoder
%
%  rx        : Received signal vector (column vector)
%  H         : LDPC matrix
%  N0        : Noise variance
%  iteration : Number of iteration
%
%  vHat      : Decoded vector (0/1) 
%
%
% Copyright Bagawan S. Nugroho, 2007 
% http://bsnugroho.googlepages.com

[M N] = size(H);
%xishu=0.8;
% Prior log-likelihood. LLR-BP法，0,1映射成-1,1
% Minus sign is used for 0/1 to -1/1 mapping
Lci = (-4*rx./N0)';

% Initialization
Lrji = zeros(M, N);
Pibetaij = zeros(M, N);

% Asscociate the L(ci) matrix with non-zero elements of H
Lqij = H.*repmat(Lci, M, 1);
 
% Get non-zero elements
[r, c] = find(H);

% Iteration
for n = 1:iteration
   
   fprintf('Iteration : %d\n', n);
   
   % Get the sign and magnitude of L(qij)   sign 符号函数变量非零则为1,反之为0
   % abs求模
   alphaij = sign(Lqij);   
   betaij = abs(Lqij);

   for l = 1:length(r)
      Pibetaij(r(l), c(l)) = betaij(r(l), c(l)); 
   end
   
   % ----- Horizontal step -----
   for i = 1:M
      
      % Find non-zeros in the column
      c1 = find(H(i, :));
      
      % Get the summation of Pi(betaij))        
      for k = 1:length(c1)

         
         prodOfalphaij = 1;
         
         % Summation of Pi(betaij)\c1(k)
         exchangk=Pibetaij(i,c1(k));
         Pibetaij(i,c1(k))=inf;
         minOfPibetaij = min(Pibetaij(i,c1));
         Pibetaij(i,c1(k))=exchangk;
         
         % Avoid division by zero/very small number, get Pi(sum(Pi(betaij)))
         %if minOfPibetaij < 1e-20
         %   minOfPibetaij = 1e-10;
        % end         
         PiminOfPibetaij = minOfPibetaij;
      
         % Multiplication of alphaij\c1(k) (use '*' since alphaij are -1/1s)
         prodOfalphaij = prod(alphaij(i, c1))*alphaij(i, c1(k));
         
         % Update L(rji)
         Lrji(i, c1(k)) = prodOfalphaij*PiminOfPibetaij;
         
      end % for k
      
   end % for i

   % ------ Vertical step ------
   for j = 1:N

      % Find non-zero in the row
      r1 = find(H(:, j));
      
      for k = 1:length(r1)        
        
         % Update L(qij) by summation of L(rij)\r1(k)
         Lqij(r1(k), j) = Lci(j) + sum(Lrji(r1, j)) - Lrji(r1(k), j);
      
      end % for k
      
      % Get L(Qi)
      LQi = Lci(j) + sum(Lrji(r1, j));
      
      % Decode L(Qi)
      if LQi < 0
         vHat(j) = 1;
      else
         vHat(j) = 0;
      end
                       
   end % for j
   
end % for n
