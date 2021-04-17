#include "StaggeredGrid.hh"


// Defination of constructor

//intializing the staggered grid with the boundary so +2
StaggeredGrid::StaggeredGrid( int xSize, int ySize, int zSize, real dx, real dy, real dz )
           :p_((xSize)+2,(ySize)+2,(zSize)+2),rhs_((xSize)+2,(ySize)+2,(zSize)+2),
            u_((xSize)+1,(ySize)+2,(zSize)+2),v_((xSize)+2,(ySize)+1,(zSize)+2),w_((xSize)+2,(ySize)+2,(zSize)+1),
            f_((xSize)+1,(ySize)+2,(zSize)+2),g_((xSize)+2,(ySize)+1,(zSize)+2),h_((xSize)+2,(ySize)+2,(zSize)+1),
            isfluid_((xSize)+2,(ySize)+2,(zSize)+2),dx_(dx), dy_(dy), dz_(dz)
{
   isfluid_.fill(255);
   p_.fill(1.0);
   u_.fill(0.0);
   v_.fill(0.0);
   w_.fill(0.0);
}

//intializing the staggered grid by reading input file
StaggeredGrid::StaggeredGrid(const FileReader & configuration):
                p_  (configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+2),
                rhs_(configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+2),
                u_  (configuration.getIntParameter("imax")+1,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+2),
                v_  (configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+1,configuration.getIntParameter("kmax")+2),
                w_  (configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+1),
                f_  (configuration.getIntParameter("imax")+1,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+2),
                g_  (configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+1,configuration.getIntParameter("kmax")+2),
                h_  (configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+1),
                isfluid_(configuration.getIntParameter("imax")+2,configuration.getIntParameter("jmax")+2,configuration.getIntParameter("kmax")+2),
                xlength(configuration.getRealParameter("xlength")), ylength(configuration.getRealParameter("ylength")), zlength(configuration.getRealParameter("zlength"))

{
    dx_=real(configuration.getRealParameter("xlength")/configuration.getIntParameter("imax"));
    dy_=real(configuration.getRealParameter("ylength")/configuration.getIntParameter("jmax"));
    dz_=real(configuration.getRealParameter("zlength")/configuration.getIntParameter("kmax"));
    
    isfluid_.fill(1);
//    int jmax_=configuration.getIntParameter("jmax");
//    int kmax_=configuration.getIntParameter("kmax");
//    for(int j = 1; j< jmax_/2  ; j++)
//        for(int k = 1; k< kmax_  ; k++)
//                isfluid_( 0,j,k) = 0;

    if (configuration.checkparameter("RectangleX1"))
       {
             createRectangle(configuration.getRealParameter("RectangleX1")/dx_ +1,
                             configuration.getRealParameter("RectangleY1")/dy_ +1,
                             configuration.getRealParameter("RectangleZ1")/dz_ +1,
                             configuration.getRealParameter("RectangleX2")/dx_ +1,
                             configuration.getRealParameter("RectangleY2")/dy_ +1,
                             configuration.getRealParameter("RectangleZ2")/dz_ +1);
//           createRectangle(configuration.getRealParameter("RectangleX1"), configuration.getRealParameter("RectangleY1"),configuration.getRealParameter("RectangleZ1"),
//                            configuration.getRealParameter("RectangleX2"), configuration.getRealParameter("RectangleY2"),configuration.getRealParameter("RectangleZ2"));
       }

//       for (int i=2; i<isfluid_.getSize(0)-1; i++)
//           for (int j=2; j<isfluid_.getSize(1)-1; j++)
//               for (int k=2; j<isfluid_.getSize(2)-1; k++)
//                    if (!isfluid_(i,j,k))
//                        CHECK_MSG(!((isfluid_(i+1,j) && isfluid_(i-1,j)) || (isfluid_(i,j+1,k) && isfluid_(i,j-1,k)) || (isfluid_(i,j,k+1) && isfluid_(i,j,k-1)) ),
//                             "Walls wrongly defined check your file. It must be minimum 2 cell thick") ;


    p_.fill(configuration.getRealParameter("P_INIT"));
    u_.fill(configuration.getRealParameter("U_INIT"));
    v_.fill(configuration.getRealParameter("V_INIT"));
    w_.fill(configuration.getRealParameter("W_INIT"));
}

void StaggeredGrid::setCellToObstacle(int x, int y, int z)
{
   isfluid_(x,y,z) = 0;
}

/*
void StaggeredGrid::createRectangle(real x1, real y1, real z1, real x2, real y2, real z2)
{
    CHECK_MSG(x1 >= 0 && x2 > x1 && y1>=0 && y2>y1 && z1>=0 && z2>z1,
              "Rectangle geometry must be specified by setting the bottom-left and top-right point");
    CHECK_MSG(x2 <= xlength && y2 <= ylength && z2 <= zlength, "Rectangle must lie inside the domain");
    for (int i=0; i<=isfluid_.getSize(0); i++)
        for (int j=0; j<=isfluid_.getSize(1); j++)
            for (int k=0; k<=isfluid_.getSize(2); k++)
                if(dx_*(i-0.5) >= x1 && dx_*(i+0.5) <= x2 && dy_*(j-0.5) >= y1 && dy_*(j+0.5) <= y2 && dz_*(k-0.5) >= z1 && dz_*(k+0.5) <= z2)
                setCellToObstacle(i,j,k);
}
*/
void StaggeredGrid::createRectangle(int x1, int y1, int z1, int x2, int y2, int z2)
    {
        for (int i = x1; i <= x2; ++i)
        for( int j = y1; j <= y2; ++j)
                for( int k = z1; k <= z2; ++k)
                    setCellToObstacle(i,j,k);
    }
