% -*- mode:octave



function d = run_network(filename,t_start,t_step,t_end,varargin)
  command = sprintf('cdn-monitor %s --time=%f:%f:%f',filename,t_start,t_step,t_end);
  format = '%f';
  if length(varargin) < 1
	error('I need a selector')
  end

  for i=1:length(varargin)
	command = sprintf('%s -m %s',command,varargin{i});
	format=sprintf('%s %%f',format);
  end
  [status data]  = unix(command);
  d = sscanf(data, "%f");
  d = reshape(d,length(varargin)+1,length(d)/(length(varargin)+1))';
end
