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

/*
 *  pmpd_core.c
 */

static inline t_float pmpd_sign(t_float v)
{
    return v > 0 ? 1 : -1;
}

static inline t_float pmpd_pow(t_float x, t_float y)
{
    return x > 0 ? pow(x,y) : -pow(-x,y);
}

static inline t_float pmpd_sqr(t_float x)
{
    return x*x ;
}

static inline t_float pmpd_mix(t_float X, t_float Y, t_float pmpd_mix)
{
    return (1-pmpd_mix)*X + pmpd_mix*Y ;
}

t_float pmpd_tabread2(t_pmpd *x, t_float pos, t_symbol *array)
{
    t_garray *a;
    int npoints;
    t_word *vec;
    t_float posx;
    
    if (!(a = (t_garray *)pd_findbyclass(array, garray_class)))
        pd_error(x, "%s: no such array", array->s_name);
    else if (!garray_getfloatwords(a, &npoints, &vec))
        pd_error(x, "%s: bad template for tabLink", array->s_name);
    else
    {
        posx = fabs(pos)*npoints;
        int n=posx;
        if (n >= npoints - 1) 
            return (pmpd_sign(pos)*vec[npoints-1].w_float);
        t_float fract = posx-n;
        return (pmpd_sign(pos) * ( fract*vec[n+1].w_float+(1-fract)*vec[n].w_float));
    }
    return( pos); // si il y a un pb sur le tableau, on renvoie l'identité
}

void pmpd_reset(t_pmpd *x)
{
    x->nb_link = 0;
    x->nb_mass = 0;
    x->minX = -1000000;
    x->maxX = 1000000;
    x->grab = 0;

}

void *pmpd_new(t_symbol *s, int argc, t_atom *argv)
{
	t_float tmp;
    t_pmpd *x = (t_pmpd *)pd_new(pmpd_class);

    pmpd_reset(x);
    
    x->main_outlet=outlet_new(&x->x_obj, 0);
    	
    x->nb_max_mass = 10000;
	x->nb_max_link = 10000;

	if((argc >= 1) && (argv[0].a_type == A_FLOAT)){
		tmp = atom_getfloatarg(0, argc, argv);
		if (tmp >= 0) {
			x->nb_max_mass = tmp;
			x->nb_max_link = tmp;
		}
	}
	if((argc >= 2) && (argv[1].a_type == A_FLOAT)){
		tmp = atom_getfloatarg(1, argc, argv);
		if (tmp >= 0) {
			x->nb_max_link = tmp;
		}
	}
	
	x->mass = getbytes(sizeof(massStruct)*x->nb_max_mass);
	x->link = getbytes(sizeof(linkStruct)*x->nb_max_link);
	
    return (void *)x;
}

void pmpd_free(t_pmpd *x){

	freebytes(x->mass, sizeof(massStruct)*x->nb_max_mass);
	freebytes(x->link, sizeof(linkStruct)*x->nb_max_link);
}

void pmpd_bang(t_pmpd *x)
{ // this part is doing all the PM
    t_float F, L, tmpX;
    t_int i;
    // post("bang");

    for (i=0; i<x->nb_mass; i++)
    // compute new masses position
        if (x->mass[i].mobile > 0) // only if mobile
        {
			// amplify force that opose to movement
			if (x->mass[i].overdamp != 0)
			{
				tmpX = x->mass[i].speedX * x->mass[i].forceX;
				tmpX = min(0,tmpX); // overdamped only if force opose movment
				tmpX *= -x->mass[i].overdamp;
				tmpX += 1;
				x->mass[i].forceX *= tmpX;
			}

            x->mass[i].speedX += x->mass[i].forceX * x->mass[i].invM;
            // x->mass[i].forceX = 0;
            x->mass[i].posX += x->mass[i].speedX ;
            // x->mass[i].posX = min(x->maxX,max(x->minX,x->mass[i].posX));
            if ( (x->mass[i].posX < x->minX) || (x->mass[i].posX > x->maxX) ) 
            {
                tmpX = min(x->maxX,max(x->minX,x->mass[i].posX));
                x->mass[i].speedX -= x->mass[i].posX - tmpX;
                x->mass[i].posX = tmpX;
            }
            x->mass[i].forceX = -x->mass[i].D2 * x->mass[i].speedX;
        }

    for (i=0; i<x->nb_link; i++)
    // compute link forces
    {
		if (x->link[i].active == 1)
        {
            L = x->link[i].mass1->posX - x->link[i].mass2->posX;
            //absL = fabs(L);
            if ( (L >= x->link[i].Lmin) && (L < x->link[i].Lmax)  && (L !=0))
            {
                if (x->link[i].lType == 2)
                { // K et D viennent d'une table
                    F  = x->link[i].D * pmpd_tabread2(x, (L - x->link[i].distance) / x->link[i].D_L, x->link[i].arrayD);
                    F += x->link[i].K * pmpd_tabread2(x, L / x->link[i].K_L, x->link[i].arrayK);
                }
                else
                {            
                    F  = x->link[i].D * (L - x->link[i].distance) ;
                    F += x->link[i].K *  pmpd_sign(L- x->link[i].L) * pmpd_pow( fabs(L - x->link[i].L), x->link[i].Pow);
                }
            
                x->link[i].mass1->forceX -= F;
                x->link[i].mass2->forceX += F;
				x->link[i].forceX = F; // save for latter use

            }
            x->link[i].distance=L;
        }
    }
}

void pmpd_mass(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
// t_symbol *Id, t_float mobile, t_float M, t_float posX)
{ // add a mass 
	if ( x->nb_mass >= x->nb_max_mass) {
		x->nb_mass = x->nb_max_mass-1; 
		pd_error(x, "pmpd masses number exceeded, please increase max masses number");
	}
    x->mass[x->nb_mass].Id = gensym("mass");
	if ((argc >= 1) &&  (argv[0].a_type == A_SYMBOL))
		x->mass[x->nb_mass].Id = atom_getsymbolarg(0,argc,argv);
	x->mass[x->nb_mass].mobile = 1;
	if ((argc >= 2) &&  (argv[1].a_type == A_FLOAT))
		x->mass[x->nb_mass].mobile = (int) atom_getfloatarg(1, argc, argv);
	t_float M = 1;
	if ((argc >= 3) &&  (argv[2].a_type == A_FLOAT))
		M = atom_getfloatarg(2, argc, argv);
	if (M<=0) M=1;
	x->mass[x->nb_mass].invM = 1/M;
	x->mass[x->nb_mass].speedX = 0;
	x->mass[x->nb_mass].posX = 0;
	if ((argc >= 4) &&  (argv[3].a_type == A_FLOAT))
		x->mass[x->nb_mass].posX = atom_getfloatarg(3, argc, argv);
	x->mass[x->nb_mass].forceX = 0;
	x->mass[x->nb_mass].num = x->nb_mass;
	x->mass[x->nb_mass].D2 = 0;
	x->mass[x->nb_mass].overdamp = 0;
	x->nb_mass++ ;
}

void pmpd_create_link(t_pmpd *x, t_symbol *Id, int mass1, int mass2, t_float K, t_float D, t_float Pow, t_float Lmin, t_float Lmax, t_int type)
{ // create a link based on mass number

    if ((x->nb_mass>0) && (mass1 != mass2) && (mass1 >= 0) && (mass2 >= 0) && (mass1 < x->nb_mass) && (mass2 < x->nb_mass) )
    {
		if ( x->nb_link >= x->nb_max_link) {
			x->nb_link = x->nb_max_link-1; 
			pd_error(x, "pmpd links number exceeded, please increase max links number");
		}
	
        x->link[x->nb_link].lType = type;
        x->link[x->nb_link].Id = Id;
        x->link[x->nb_link].active = 1;
        x->link[x->nb_link].mass1 = &x->mass[mass1]; 
        x->link[x->nb_link].mass2 = &x->mass[mass2];
        x->link[x->nb_link].K = K;
        x->link[x->nb_link].D = D;
        x->link[x->nb_link].L = x->mass[mass1].posX - x->mass[mass2].posX;
        x->link[x->nb_link].Pow = Pow;
        x->link[x->nb_link].Lmin = Lmin;
        x->link[x->nb_link].Lmax = Lmax;
        x->link[x->nb_link].distance =  x->link[x->nb_link].L;
        x->link[x->nb_link].forceX = 0 ;

        x->nb_link++ ;
    }
}

void pmpd_link(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{ // add a link : Id, *mass1, *mass2, K, D, Pow, Lmin, Lmax;

    int i, j;

	t_symbol *Id = gensym("link");
	if ((argc >= 1) &&  (argv[0].a_type == A_SYMBOL))
		Id = atom_getsymbolarg(0,argc,argv);
	t_float K = 0;
   	if ((argc >= 4) &&  (argv[3].a_type == A_FLOAT))
		K = atom_getfloatarg(3, argc, argv);
	t_float D = 0;
   	if ((argc >= 5) &&  (argv[4].a_type == A_FLOAT))
		D = atom_getfloatarg(4, argc, argv);
    t_float Pow = 1; 
    if (argc > 5) Pow = atom_getfloatarg(5, argc, argv);
    t_float Lmin = -1000000;
    if (argc > 6) Lmin = atom_getfloatarg(6, argc, argv);
    t_float Lmax =  1000000;
    if (argc > 7) Lmax = atom_getfloatarg(7, argc, argv);
//    post("%d,%d, %f,%f", mass1, mass2, K, D);

    if ( ( argc > 2 ) && ( argv[1].a_type == A_FLOAT ) && ( argv[2].a_type == A_FLOAT ) )
    {
        pmpd_create_link(x, Id, atom_getfloatarg(1, argc, argv), atom_getfloatarg(2, argc, argv), K, D, Pow, Lmin, Lmax, 0);
    }
    else if ( ( argc > 2 ) && ( argv[1].a_type == A_SYMBOL ) && ( argv[2].a_type == A_FLOAT ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            if ( atom_getsymbolarg(1,argc,argv) == x->mass[i].Id)
            {
                pmpd_create_link(x, Id, i, atom_getfloatarg(2, argc, argv), K, D, Pow, Lmin, Lmax, 0);
            }
        }
    }
    else if ( ( argc > 2 ) && ( argv[1].a_type == A_FLOAT ) && ( argv[2].a_type == A_SYMBOL ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            if ( atom_getsymbolarg(2,argc,argv) == x->mass[i].Id)
            {
                pmpd_create_link(x, Id, atom_getfloatarg(1, argc, argv), i, K, D, Pow, Lmin, Lmax, 0);
            }
        }
    }
    else if ( ( argc > 2 ) && ( argv[1].a_type == A_SYMBOL ) && ( argv[2].a_type == A_SYMBOL ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            for (j=0; j< x->nb_mass; j++)
            {
                if ( (atom_getsymbolarg(1,argc,argv) == x->mass[i].Id) && (atom_getsymbolarg(2,argc,argv) == x->mass[j].Id))
                {
                    if (!( (x->mass[i].Id == x->mass[j].Id) && (i>j) )) 
                    // si lien entre 2 serie de masses identique entres elle, alors on ne creer qu'un lien sur 2, pour evider les redondances
                        pmpd_create_link(x, Id, i, j, K, D, Pow, Lmin, Lmax, 0);
                }
            }   
        }
    }
    else
		pmpd_create_link(x, Id, 0, 1, K, D, Pow, Lmin, Lmax, 0);
}

void pmpd_tabLink(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, j;
    
    t_symbol *Id = atom_getsymbolarg(0,argc,argv);
    int mass1 = atom_getfloatarg(1, argc, argv);
    int mass2 = atom_getfloatarg(2, argc, argv);
    t_symbol *arrayK = atom_getsymbolarg(3,argc,argv);
    t_float Kl = atom_getfloatarg(4, argc, argv);
    if (Kl <= 0) Kl = 1;
    t_symbol *arrayD = atom_getsymbolarg(5,argc,argv);    
    t_float Dl = atom_getfloatarg(6, argc, argv);
    if (Dl <= 0) Dl = 1;
    
    if ( ( argv[1].a_type == A_FLOAT ) && ( argv[2].a_type == A_FLOAT ) )
    {
        pmpd_create_link(x, Id, mass1, mass2, 1, 1, 1, -1000000, 1000000, 2);
        x->link[x->nb_link-1].arrayK = arrayK;
        x->link[x->nb_link-1].arrayD = arrayD;
        x->link[x->nb_link-1].K_L = Kl;
        x->link[x->nb_link-1].D_L = Dl;    
    }
    else
    if ( ( argv[1].a_type == A_SYMBOL ) && ( argv[2].a_type == A_FLOAT ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            if ( atom_getsymbolarg(1,argc,argv) == x->mass[i].Id)
            {
                pmpd_create_link(x, Id, i, mass2, 1, 1, 1, -1000000, 1000000, 2);
                x->link[x->nb_link-1].arrayK = arrayK;
                x->link[x->nb_link-1].arrayD = arrayD;
                x->link[x->nb_link-1].K_L = Kl;
                x->link[x->nb_link-1].D_L = Dl;    
            }
        }
    }
    else
    if ( ( argv[1].a_type == A_FLOAT ) && ( argv[2].a_type == A_SYMBOL ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            if ( atom_getsymbolarg(2,argc,argv) == x->mass[i].Id)
            {
                pmpd_create_link(x, Id, mass1, i, 1, 1, 1, -1000000, 1000000, 2);
                x->link[x->nb_link-1].arrayK = arrayK;
                x->link[x->nb_link-1].arrayD = arrayD;
                x->link[x->nb_link-1].K_L = Kl;
                x->link[x->nb_link-1].D_L = Dl;    
            }
        }
    }
    else
    if ( ( argv[1].a_type == A_SYMBOL ) && ( argv[2].a_type == A_SYMBOL ) )
    {
        for (i=0; i< x->nb_mass; i++)
        {
            for (j=0; j< x->nb_mass; j++)
            {
                if ( (atom_getsymbolarg(1,argc,argv) == x->mass[i].Id) && (atom_getsymbolarg(2,argc,argv) == x->mass[j].Id))
                {
                    if (!( (x->mass[i].Id == x->mass[j].Id) && (i>j) )) 
                    // si lien entre 2 serie de masses identique entres elle, alors on ne creer qu'un lien sur 2, pour evider les redondances
                    {
                        pmpd_create_link(x, Id, i, j, 1, 1, 1, -1000000, 1000000, 2);
                        x->link[x->nb_link-1].arrayK = arrayK;
                        x->link[x->nb_link-1].arrayD = arrayD;
                        x->link[x->nb_link-1].K_L = Kl;
                        x->link[x->nb_link-1].D_L = Dl;    
                    }
                }
            }   
        }
    }
}


void pmpd_delLink_int(t_pmpd *x, int dellink)
{
	int i;
	if ( ( dellink < x->nb_link ) && ( dellink >= 0) )
	{
		x->nb_link--;
		for (i=dellink; i < x->nb_link; i++)
		x->link[i]=x->link[i+1]; 
	}
}

void pmpd_delLink(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	int i,nb_toremove;
	if ( (argc > 0) && ( argv[0].a_type == A_FLOAT ) )
		pmpd_delLink_int(x, atom_getfloatarg(0, argc, argv));
	if ( (argc > 0) && ( argv[0].a_type == A_SYMBOL ) )
/*		for (i=0; i<x->nb_link; )
			if ( atom_getsymbolarg(0,argc,argv) == x->link[i].Id )
				pmpd_delLink_int(x, i);
			else i++;
*/
	{
		nb_toremove=0;
		for (i=0; i < x->nb_link; i++)
		{
			if ( atom_getsymbolarg(0,argc,argv) == x->link[i].Id )
			{
				nb_toremove++;
			}
			else			
			{	
				if (nb_toremove > 0)
				{
					x->link[i-nb_toremove]=x->link[i]; 
				}
			}
		}
		x->nb_link -= nb_toremove;
	}
}

void pmpd_delMass_int(t_pmpd *x, int delmass)
{
	int i,nb_toremove;

	if ( ( delmass < x->nb_mass ) && ( delmass >= 0) )
	{
	/*	for (i=0; i < x->nb_link; ) // delete link connected to the mass to delete
		{
			if ( (x->link[i].mass1->num == delmass) || (x->link[i].mass2->num == delmass) )
			pmpd_delLink_int(x, i);
			else i++;
			// post("loop %d sur %d", i, x->nb_link);
		}*/
		nb_toremove=0;
		for (i=0; i < x->nb_link; i++)
		{
			if ( (x->link[i].mass1->num == delmass) || (x->link[i].mass2->num == delmass) )
			{
				nb_toremove++;
			}
			else			
			{	
				if (nb_toremove > 0)
				{
					x->link[i-nb_toremove]=x->link[i]; 
				}
			}
		}
		x->nb_link -= nb_toremove;
		
		for (i=0; i < x->nb_link; i++) // change pointer to mass that index moved
		{
			if (x->link[i].mass1->num > delmass )
			{ x->link[i].mass1 = &x->mass[x->link[i].mass1->num-1]; }
			if (x->link[i].mass2->num > delmass )
			{ x->link[i].mass2 = &x->mass[x->link[i].mass2->num-1]; }
		}
		x->nb_mass--;
		for (i=delmass; i < x->nb_mass; i++)
		{
			x->mass[i]=x->mass[i+1];
			x->mass[i].num=i;
		}
	}
}

void pmpd_delMass(t_pmpd *x, t_symbol *s, int argc, t_atom *argv)
{
	int i, j, nb_toremove;
	if ( (argc > 0) && ( argv[0].a_type == A_FLOAT ) )
		pmpd_delMass_int(x, atom_getfloatarg(0, argc, argv));
	if ( (argc > 0) && ( argv[0].a_type == A_SYMBOL ) )
	/*
		for (i=0; i<x->nb_mass; )
			if ( atom_getsymbolarg(0,argc,argv) == x->mass[i].Id )
				pmpd_delMass_int(x, i);
			else i++;
	*/
	{
		nb_toremove=0;
		for (i=0; i < x->nb_link; i++) // revove link associated with this mass Id
		{
			if ( (x->link[i].mass1->Id == atom_getsymbolarg(0, argc, argv)) || (x->link[i].mass2->Id == atom_getsymbolarg(0, argc, argv)) )
			{
				nb_toremove++;
			}
			else			
			{	
				if (nb_toremove > 0)
				{
					x->link[i-nb_toremove]=x->link[i]; 
				}
			}
		}
		x->nb_link -= nb_toremove;
		
		nb_toremove=0;
		for (i=0; i < x->nb_mass; i++) // remove mass
		{
			if ( atom_getsymbolarg(0,argc,argv) == x->mass[i].Id )
			{
				nb_toremove++;
				// post("remove mass %d", i);
			}
			else			
			{	
				if (nb_toremove > 0)
				{
					x->mass[i-nb_toremove]=x->mass[i]; 
					x->mass[i-nb_toremove].num = i-nb_toremove;
					for (j=0; j < x->nb_link; j++) // for every link replace mass with the new pointer
					{
						if (x->link[j].mass1->num == i )
						{ 
							// post("mass %d : relocate link %d to mass %d",i, j, i-nb_toremove);
							x->link[j].mass1 = &x->mass[i-nb_toremove]; 
						}
						else if (x->link[j].mass2->num == i )
						{ 
							// post("mass %d : relocate link2 %d to mass %d",i, j, i-nb_toremove);
							x->link[j].mass2 = &x->mass[i-nb_toremove];				
						}
					}
				}
			}
		}
		x->nb_mass -= nb_toremove;
	}
}
