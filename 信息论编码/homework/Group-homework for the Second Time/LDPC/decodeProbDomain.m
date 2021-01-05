function vHat = decodeProbDomain(rx, H, N0, iteration)
% Probability-domain sum product algorithm LDPC decoder
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

% Prior probabilities
P1 = ones(size(rx))./(1 + exp(-2*rx./(N0/2)));
P0 = 1 - P1;

% Initialization
K0 = zeros(M, N);
K1 = zeros(M, N);
rji0 = zeros(M, N);
rji1 = zeros(M, N);
qij0 = H.*repmat(P0', M, 1);
qij1 = H.*repmat(P1', M, 1);

% Iteration
for n = 1:iteration
   
   fprintf('Iteration : %d\n', n);
   
   % ----- Horizontal step -----
   for i = 1:M
      
      % Find non-zeros in the column
      c1 = find(H(i, :));
      
      for k = 1:length(c1)
         
         % Get column products of drji\c1(l)
         drji = 1;
         for l = 1:length(c1)
            if l~= k
               drji = drji*(qij0(i, c1(l)) - qij1(i, c1(l)));
            end
         end % for l
         
         rji0(i, c1(k)) = (1 + drji)/2;
         rji1(i, c1(k)) = (1 - drji)/2;
         
      end % for k
      
   end % for i
   
   % ------ Vertical step ------
   for j = 1:N
      
      % Find non-zeros in the row
      r1 = find(H(:, j));
      
      for k = 1:length(r1)
        
         % Get row products of prodOfrij\ri(l)
         prodOfrij0 = 1;
         prodOfrij1 = 1;   
         for l = 1:length(r1)
            if l~= k
               prodOfrij0 = prodOfrij0*rji0(r1(l), j);
               prodOfrij1 = prodOfrij1*rji1(r1(l), j);
            end
         end % for l
         
         % Update constants
         K0(r1(k), j) = P0(j)*prodOfrij0;
         K1(r1(k), j) = P1(j)*prodOfrij1;
         
         % Update qij0 and qij1
         qij0(r1(k), j) = K0(r1(k), j)./(K0(r1(k), j) + K1(r1(k), j));
         qij1(r1(k), j) = K1(r1(k), j)./(K0(r1(k), j) + K1(r1(k), j));
               
      end % for k
      
      % Update constants
      Ki0 = P0(j)*prod(rji0(r1, j));
      Ki1 = P1(j)*prod(rji1(r1, j));
      
      % Get Qj
      Qi0 = Ki0/(Ki0 + Ki1);
      Qi1 = Ki1/(Ki0 + Ki1);
      
      % Decode Qj        
      if Qi1 > Qi0
         vHat(j) = 1;
      else
         vHat(j) = 0;
      end
         
   end % for j
   
end % for n

