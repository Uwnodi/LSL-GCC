s/_START "([^"]*)" _END/"\1"/g
s/_START ([0-9\.]+) _END/"\1"/g
s/_START ([^"]*) _END/"" + (string)(\1) + ""/g
s/\(string\)"([^"]*)"/"\1"/g
s/" \+ "//g
s/" ~ "//g
