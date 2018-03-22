% -*- mode:octave -*-
clear all


function [d,power,power_max,i_max,i_min,v_max,v_min] = analyze(C,R)
  fid = fopen('search.cdn', 'w');
  fprintf(fid,'include "comparative_study_current_limiter.cdn"\nnode RCLimiter {\nC = %f\nR = %f\n}\n',C,R);
  fclose(fid);
  t_start = 0;
  t_step = 10e-6;
  t_end = 0.2;
  period = 3*0.03;
  n_samples = period / t_step;

  d = run_network( 'search.cdn',0,10e-6,1, 'RCLimiter.I_R','RCLimiter.V', 'RCLimiter.P_lim');
  idx=(size(d,1) - n_samples):size(d,1);
  power_max = max(d(idx,4));
  power = mean(d(idx,4));
  v_max = max(d(idx,3));
  v_min = min(d(idx,3));
  i_max = max(d(idx,2));
  i_min = min(d(idx,2));
end

Cvalues = [47e-6 470e-6 680e-6 1e-3 2.2e-3 4.7e-3 6.8e-3];
Rvalues = [0.5 1 2.21 4.75 6.81 10];


try
  load('cache.mat','all_data')

  if size(all_data.power,1) ~= length(Cvalues) || size(all_data.power,2) ~= length(Rvalues)
	error 'retry'
  end

catch

  all_data.power = [];
  all_data.power_max = [];
  all_data.i_max = [];
  all_data.i_min = [];
  all_data.v_max = [];
  all_data.v_min = [];

  h = waitbar (0, '0.00%');
  i = 0;
  nb = length(Cvalues) * length(Rvalues);
  for cIdx=1:length(Cvalues)
	for rIdx=1:length(Rvalues)
	  C = Cvalues(cIdx);
	  R = Rvalues(rIdx);
	  [d,...
	   all_data.power(cIdx,rIdx),...
	   all_data.power_max(cIdx,rIdx),...
	   all_data.i_max(cIdx,rIdx),...
	   all_data.i_min(cIdx,rIdx),...
	   all_data.v_max(cIdx,rIdx),...
	   all_data.v_min(cIdx,rIdx)] = analyze(C,R);
	  i = i + 1;
	  waitbar(i/nb,h,sprintf('Done %d of %d',i,nb));
	end
  end
  close(h)
  save('cache.mat','all_data')
end_try_catch


figure(1)
subplot(2,2,1)
surf(Rvalues,Cvalues,all_data.power);
hold on
mesh(Rvalues,Cvalues,all_data.power_max);
hold off

xlabel('resistance (\ohm)')
ylabel('Capacitance (F)')
zlabel('Power (W)')
set(gca,'ZScale','log')


subplot(2,2,2)
surf(Rvalues,Cvalues,all_data.i_max - all_data.i_min)
set(gca,'ZScale','log')


subplot(2,2,3)
surf(Rvalues,Cvalues,all_data.v_max - all_data.v_min)
set(gca,'ZScale','log')

subplot(2,2,4)
surf(Rvalues,Cvalues,1.0./(all_data.power .* (all_data.i_max - all_data.i_min)))
set(gca,'ZScale','log')
