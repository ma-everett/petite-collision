
source = Split( """
  petitecollision.c
  """ )
  
Library(target='petitecollision', source=source);


libs = Split( """
  petitecollision
  pthread  
  m
  dl
	""")
  


Program(target='pcol',LIBS=libs,LIBPATH='.',source='main.c');


