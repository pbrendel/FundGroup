F:=FreeGroup(2);
g:=GeneratorsOfGroup(F);
rels:=[];
w:=g[1]*g[2]*g[1]^-1*g[2]*g[1]*g[2]^-1; Add(rels,w);
w:=g[2]^-1*g[1]*g[2]^-1*g[1]^-1*g[2]*g[1]^-1; Add(rels,w);
G:=PresentationFpGroup(F/rels);
SimplifyPresentation(G);
P:=FpGroupPresentation(G);
R:=RelatorsOfFpGroup(P);
L:=LowIndexSubgroupsFpGroup(P,10);;
K:=List(L,AbelianInvariants);;
Sort(K);

