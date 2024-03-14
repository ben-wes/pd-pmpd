// --------------------------------------------------------------------------
// This file is part of the pmpd software.
//
//    pmpd software is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    pmpd firmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with pmpd software. If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------- 
//
// pmpd = physical modeling for pure data
// ch@chnry.net


int pmpd_test_1d_mass(int i, t_pmpd *x, int argc, t_atom *argv)
{
	t_float tmp;
	t_int j = 1, k;
	while (j < argc) 
	{
		if (argv[j].a_type != A_SYMBOL) 
		{ 
			j++;
		}
		else
		{
			if (atom_getsymbolarg(j,argc,argv) == gensym("Id") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_SYMBOL) ) return(0); 
				if ( x->mass[i].Id != atom_getsymbolarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
            else if (atom_getsymbolarg(j,argc,argv) == gensym("mobile") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].mobile != atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("posXSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].posX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("posXInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].posX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("speedXSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].speedX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("speedXInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].speedX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("speedSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].speedX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+= 2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("speedInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].speedX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("distanceInf"))
			{ 
				if ( (j+2 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if (argv[j+2].a_type != A_FLOAT) return(0); 
				tmp = pmpd_sqr(x->mass[i].posX - atom_getfloatarg(j+1,argc,argv));
				if ( tmp >= pmpd_sqr(atom_getfloatarg(j+2,argc,argv)) ) { return(0); }
				j += 3;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("distanceSup"))
			{ 
				if ( (j+2 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if (argv[j+2].a_type != A_FLOAT) return(0); 
				tmp = pmpd_sqr(x->mass[i].posX - atom_getfloatarg(j+1,argc,argv));
				if ( tmp < pmpd_sqr(atom_getfloatarg(j+2,argc,argv)) ) { return(0); }
				j += 3;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("massSup"))
			{ 
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( 1/ x->mass[i].invM < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j += 2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("massInf"))
			{ 
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( 1/ x->mass[i].invM >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j += 2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("forceSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].forceX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+= 2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("forceInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].forceX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+= 2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("forceXSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].forceX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("forceXInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->mass[i].forceX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkSup") )
			{ // link number
				tmp=0;
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].mass1->num == x->mass[i].num) ||  (x->link[k].mass2->num == x->mass[i].num) ) tmp++;
				}
				if ( tmp <= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkInf") )
			{ // link number
				tmp=0;
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].mass1->num == x->mass[i].num) ||  (x->link[k].mass2->num == x->mass[i].num) ) tmp++;
				}
				if ( tmp >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkEqual") )
			{ // link number
				tmp=0;
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].mass1->num == x->mass[i].num) ||  (x->link[k].mass2->num == x->mass[i].num) ) tmp++;
				}
				if ( tmp != atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkNameSup") )
			{ // link name, link number
				tmp=0;
				if ( (j+2 >=  argc) || (argv[j+1].a_type != A_SYMBOL) ) return(0); 
				if (argv[j+1].a_type != A_FLOAT) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].Id ==  atom_getsymbolarg(j+1,argc,argv)) && ((x->link[k].mass2->num == x->mass[i].num) ||  (x->link[k].mass1->num == x->mass[i].num)) ) tmp++;
				}
				if ( tmp <= atom_getfloatarg(j+2,argc,argv) ) { return(0); }
				j+=3;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkNameInf") )
			{ // link name, link number
				tmp=0;
				if ( (j+2 >=  argc) || (argv[j+1].a_type != A_SYMBOL) ) return(0); 
				if (argv[j+1].a_type != A_FLOAT) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].Id ==  atom_getsymbolarg(j+1,argc,argv)) && ((x->link[k].mass2->num == x->mass[i].num) ||  (x->link[k].mass1->num == x->mass[i].num)) ) tmp++;
				}
				if ( tmp >= atom_getfloatarg(j+2,argc,argv) ) { return(0); }
				j+=3;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("nbLinkNameEqual") )
			{ // link name, link number
				tmp=0;
				if ( (j+2 >=  argc) || (argv[j+1].a_type != A_SYMBOL) ) return(0); 
				if (argv[j+1].a_type != A_FLOAT) return(0); 
				for (k=0; k < x->nb_link; k++)
				{
					if ( (x->link[k].Id ==  atom_getsymbolarg(j+1,argc,argv)) && ((x->link[k].mass2->num == x->mass[i].num) ||  (x->link[k].mass1->num == x->mass[i].num)) ) tmp++;
				}
				if ( tmp != atom_getfloatarg(j+2,argc,argv) ) { return(0); }
				j+=3;
			}
			else
			{
				pd_error(x,"Option \"%s\" not recognized, stopping test",atom_getsymbolarg(j,argc,argv)->s_name);
				return(-1);
			}
		}
	}
	return(1);	
}

int pmpd_test_1d_link(int i, t_pmpd *x, int argc, t_atom *argv)
{
	t_int j;
	t_float tmp;
	j = 1;
	
	while (j < argc) 
	{
		if (argv[j].a_type != A_SYMBOL) 
		{ 
			j++;
		}
		else
		{
			if (atom_getsymbolarg(j,argc,argv) == gensym("Id") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_SYMBOL) ) return(0); 
				if ( x->link[i].Id != atom_getsymbolarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}			
            else if (atom_getsymbolarg(j,argc,argv) == gensym("active") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].active != atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("forceXSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].forceX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("forceXInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].forceX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("forceSup") )
			{	
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].forceX < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("forceInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].forceX >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if (atom_getsymbolarg(j,argc,argv) == gensym("lengthSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].distance < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("lengthInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				if ( x->link[i].distance >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("lengthXInf") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				tmp = fabs(x->link[i].mass1->posX - x->link[i].mass2->posX);
				if ( tmp >= atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("lengthXSup") )
			{
				if ( (j+1 >=  argc) || (argv[j+1].a_type != A_FLOAT) ) return(0); 
				tmp = fabs(x->link[i].mass1->posX - x->link[i].mass2->posX);
				if ( tmp < atom_getfloatarg(j+1,argc,argv) ) { return(0); }
				j+=2;
			}		
			else if ( atom_getsymbolarg(j,argc,argv) == gensym("connectedTo") )
			{
				if (j+1 >=  argc) return(0); 
				if (argv[j+1].a_type == A_SYMBOL)
				{
					if (!( (x->link[i].mass1->Id == atom_getsymbolarg(j+1,argc,argv)) || (x->link[i].mass2->Id == atom_getsymbolarg(j+1,argc,argv)) )) 
					{ return(0); }
					j+=2;				
				}
				else if (argv[j+1].a_type == A_FLOAT)
				{
					if (!( (x->link[i].mass1->num == atom_getfloatarg(j+1,argc,argv)) || (x->link[i].mass2->num == atom_getfloatarg(j+1,argc,argv)) ))
					{ return(0); }
					j+=2;
				}
				else
				{
					pd_error(x,"bad argument for connectedTo");
					j+=1;
				}
			}
			else
			{
				pd_error(x,"Option \"%s\" not recognized, stopping test",atom_getsymbolarg(j,argc,argv)->s_name);
				return(-1);
			}
		}
	}
	return(1);
}

//----------------------------------------------------------------------

void pmpd_testLink(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_int i, tmp;
	t_atom std_out[2];

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));

	for (i=0; i < x->nb_link; i++)
	{
		tmp=pmpd_test_1d_link(i,x,argc,argv);
		if (tmp == -1)
		{	
			break;
		}
		else if (tmp)
		{
			SETFLOAT(&(std_out[1]),i);
			outlet_anything(x->main_outlet, gensym("testLink"),2,std_out);
		}
	}
}

void pmpd_testMass(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_int i, tmp;
	t_atom std_out[2];

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));

	for (i=0; i < x->nb_mass; i++)
	{
		tmp=pmpd_test_1d_mass(i,x,argc,argv);
		if (tmp == -1)
		{	
			break;
		}
		else if (tmp)
		{
			SETFLOAT(&(std_out[1]),i);
			outlet_anything(x->main_outlet, gensym("testMass"),2,std_out);
		}
	}
}

void pmpd_testLinkT(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, vecsize, tmp;
    t_garray *a;
    t_word *vec;
    
    if (argv[0].a_type == A_SYMBOL)
    {
        t_symbol *tab_name = atom_getsymbolarg(0, argc, argv);
        if (!(a = (t_garray *)pd_findbyclass(tab_name, garray_class)))
            pd_error(x, "%s: no such array", tab_name->s_name);
        else if (!garray_getfloatwords(a, &vecsize, &vec))
            pd_error(x, "%s: bad template for tabwrite", tab_name->s_name);
        else
        {
			int taille_max = x->nb_link;
            taille_max = min(taille_max, vecsize);
            
            for (i=0; i < taille_max; i++)
            {
				tmp = pmpd_test_1d_link(i,x,argc,argv);
				if (tmp == -1)
				{	
					break;
				}
				vec[i].w_float = (t_float)tmp;
            }
            garray_redraw(a);
        }
    }
}

void pmpd_testMassT(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, vecsize, tmp;
    t_garray *a;
    t_word *vec;
    
    if (argv[0].a_type == A_SYMBOL)
    {
        t_symbol *tab_name = atom_getsymbolarg(0, argc, argv);
        if (!(a = (t_garray *)pd_findbyclass(tab_name, garray_class)))
            pd_error(x, "%s: no such array", tab_name->s_name);
        else if (!garray_getfloatwords(a, &vecsize, &vec))
            pd_error(x, "%s: bad template for tabwrite", tab_name->s_name);
        else
        {
			int taille_max = x->nb_mass;
            taille_max = min(taille_max, vecsize);
            
            for (i=0; i < taille_max; i++)
            {
				tmp = pmpd_test_1d_mass(i,x,argc,argv);
				if (tmp == -1)
				{
					break;
				}
				vec[i].w_float = (t_float)tmp;
            }
            garray_redraw(a);
        }
    }
}

void pmpd_testLinkL(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, tmp;
    t_atom* list = (t_atom*)malloc((x->nb_link + 1)* sizeof(t_atom));

    for (i=0; i < x->nb_link;)
	{
		tmp=pmpd_test_1d_link(i,x,argc,argv);
		if (tmp == -1)
		{
			break;
		}
		SETFLOAT(&(list[i+1]), (t_float)tmp);
		i++;
	}
	SETSYMBOL(&(list[0]),atom_getsymbolarg(0,argc,argv));
	outlet_anything(x->main_outlet, gensym("testLinkL"),i+1 , list);

	free(list);
}

void pmpd_testMassL(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, tmp;
    t_atom* list = (t_atom*)malloc((x->nb_mass + 1)* sizeof(t_atom));

    for (i=0; i < x->nb_mass;)
	{		
		tmp=pmpd_test_1d_mass(i,x,argc,argv);
		if (tmp == -1)
		{
			break;
		}
		SETFLOAT(&(list[i+1]), (t_float)tmp);
		i++;
	}
	SETSYMBOL(&(list[0]),atom_getsymbolarg(0,argc,argv));
	outlet_anything(x->main_outlet, gensym("testMassL"),i+1 , list);

	free(list);
}

void pmpd_testLinkN(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_atom std_out[3];
	t_int i, tmp;

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));
    i = atom_getfloatarg(1,argc,argv);
    i = min(x->nb_link-1,i);
    i = max(0,i);
    SETFLOAT(&(std_out[1]),i);

    tmp=pmpd_test_1d_link(i,x,argc,argv);
    if (tmp == -1)
    {	
        return;
    }
    if (tmp)
        SETFLOAT(&(std_out[2]),1);
    else
        SETFLOAT(&(std_out[2]),0);

    outlet_anything(x->main_outlet, gensym("testLinkN"),3,std_out);
}

void pmpd_testMassN(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_atom std_out[3];
	t_int i, tmp;

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));
    i = atom_getfloatarg(1,argc,argv);
    i = min(x->nb_mass-1,i);
    i = max(0,i);
    SETFLOAT(&(std_out[1]),i);

    tmp=pmpd_test_1d_mass(i,x,argc,argv);
    if (tmp == -1)
    {	
        return;
    }
    if (tmp)
        SETFLOAT(&(std_out[2]),1);
    else
        SETFLOAT(&(std_out[2]),0);

    outlet_anything(x->main_outlet, gensym("testMassN"),3,std_out);
}

void pmpd_testLinkNumber(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_int i, tmp, nb;
	t_atom std_out[2];

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));

    nb=0;
	for (i=0; i < x->nb_link; i++)
	{
		tmp=pmpd_test_1d_link(i,x,argc,argv);
		if (tmp == -1)
		{	
			break;
		}
		else if (tmp)
		{
            nb++;
		}
	}
    SETFLOAT(&(std_out[1]),nb);
    outlet_anything(x->main_outlet, gensym("testLinkNumber"),2,std_out);
}

void pmpd_testMassNumber(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	t_int i, tmp, nb;
	t_atom std_out[2];

	SETSYMBOL(&(std_out[0]),atom_getsymbolarg(0,argc,argv));

    nb = 0;
	for (i=0; i < x->nb_mass; i++)
	{
		tmp=pmpd_test_1d_mass(i,x,argc,argv);
		if (tmp == -1)
		{	
			break;
		}
		else if (tmp)
		{
            nb++;
		}
	}
    SETFLOAT(&(std_out[1]),nb);
	outlet_anything(x->main_outlet, gensym("testMassNumber"),2,std_out);
}
