s/_START "([^"]*)" _END/"\1"/g
s/_START ([0-9\.]+) _END/"\1"/g

# Convert _START..._END patterns to string concatenation syntax
s/_START ([^"]*) _END/"" + (string)(\1) + ""/g

# Simplify redundant string casting syntax
s/\(string\)\s?"([^"]*)"/"\1"/g

# Remove string concatenation operators (plus format)
s/" \+ "//g

# Remove string concatenation operators (tilde format)
s/" ~ "//g
