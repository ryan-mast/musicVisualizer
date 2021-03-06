#include "BezierPatch4.h"

// default constructor
BezierPatch4::BezierPatch4()
{
  amplitude = 0;
    // convert control points into array of floats
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        Vector3 currentVec(i,0,j);
        ctrlpoints[i][j][0] = currentVec.get(0);
        ctrlpoints[i][j][1] = currentVec.get(1);
        ctrlpoints[i][j][2] = currentVec.get(2);

        // set initial color to white
        colors[i][j][0] = 0;
        colors[i][j][1] = 0;
        colors[i][j][2] = 0;
        colors[i][j][3] = 0;
      }
    }

 //   glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MAP2_VERTEX_3);
 //   glEnable(GL_MAP2_COLOR_4);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_AUTO_NORMAL);
}

// constructor that takes the corners and makes a flat patch (Y is always 0)
BezierPatch4::BezierPatch4(Vector3 topLeft, Vector3 topRight, Vector3 lowerBound)
{
  // amplitude is the middle (average of the 3 points' y)
  amplitude = (topLeft.get(1)+topRight.get(1)+lowerBound.get(1))/3.0;

  // interpolates the points to create the surface
  Vector3 deltaX(topRight - topLeft); // how much the x direction changes
  deltaX.scale(1.0 / 3);
  Vector3 deltaZ(lowerBound - topLeft); // how much the z direction changes
  deltaZ.scale(1.0 / 3);

  // convert control points into array of floats
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // find current interpolated point
      Vector3 currX(deltaX);
      currX.scale(i);
      Vector3 currZ(deltaZ);
      currZ.scale(j);
      Vector3 currentVec = topLeft + currX + currZ;

      // set control point accordingly
      ctrlpoints[i][j][0] = currentVec.get(0);
      ctrlpoints[i][j][1] = currentVec.get(1);
      ctrlpoints[i][j][2] = currentVec.get(2);

      // set initial color to white
      colors[i][j][0] = 0;
      colors[i][j][1] = 0;
      colors[i][j][2] = 0;
      colors[i][j][3] = 0;
    }
  }
 // glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_MAP2_VERTEX_3);
 // glEnable(GL_MAP2_COLOR_4);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
}



// Bezier patch constructor
BezierPatch4::BezierPatch4(Vector3 * controlPoints)
{  
  // convert control points into array of floats
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Vector3 currentVec = controlPoints[i*4 + j];
      ctrlpoints[i][j][0] = currentVec.get(0);
      ctrlpoints[i][j][1] = currentVec.get(1);
      ctrlpoints[i][j][2] = currentVec.get(2);

      // set initial color to white
      colors[i][j][0] = 0;
      colors[i][j][1] = 0;
      colors[i][j][2] = 0;
      colors[i][j][3] = 0;

      // add to amplitude
      amplitude += currentVec.get(1);
    }
  }

  // amplitude is an average of all the points
  amplitude /= NUM_CONTROL_POINTS;

  glEnable(GL_MAP2_VERTEX_3);
//  glEnable(GL_MAP2_COLOR_4);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
}

BezierPatch4::~BezierPatch4()
{

}


// takes array to 16 Vector3s and changes the control points
void BezierPatch4::setControlPoints(Vector3* controlPoints)
{

}

// changes the value of a single control point
void BezierPatch4::setControlPoint(int m, int n, Vector3 vec)
{
  ctrlpoints[m][n][0] = vec.get(0);
  ctrlpoints[m][n][1] = vec.get(1);
  ctrlpoints[m][n][2] = vec.get(2);
}

// changes the value of a single control point
Vector3 BezierPatch4::getControlPoint(int m, int n)
{
  return Vector3(ctrlpoints[m][n][0], ctrlpoints[m][n][1], ctrlpoints[m][n][2]);
}

// takes a color map and sets each points color based on it's amp
void BezierPatch4::setColors(ColorGradient* colorMap, double yScale)
{
  // loop through every control point
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // set the color based on the yscale
      Vector4 newColor = colorMap->getColor(ctrlpoints[i][j][1] / yScale);

     /* colors[i][j][0] = newColor[0];
      colors[i][j][1] = newColor[1];
      colors[i][j][2] = newColor[2];
      colors[i][j][3] = newColor[3]; */
      
      colors[i][j][0] = 0;
      colors[i][j][1] = 1;
      colors[i][j][2] = 1;
      colors[i][j][3] = 1;
    }
  }
}


// changes the color of a single control point
void BezierPatch4::setColor(int m, int n, Vector4 vec)
{
  colors[m][n][0] = vec.get(0);
  colors[m][n][1] = vec.get(1);
  colors[m][n][2] = vec.get(2);
  colors[m][n][3] = vec.get(3);
}

// gets the color of a single control point
Vector4 BezierPatch4::getColor(int m, int n)
{
  return Vector4(colors[m][n][0], colors[m][n][1], colors[m][n][2], colors[m][n][3]);
}

/*
* copyColor
* copies the color and the respective y values of
* the given patch to the current patch.
*/
void BezierPatch4::copyColor(BezierPatch4 other)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      colors[i][j][0] = other.getColor(i, j)[0];
      colors[i][j][1] = other.getColor(i, j)[1];
      colors[i][j][2] = other.getColor(i, j)[2];
      colors[i][j][3] = other.getColor(i, j)[3];
    }
  }
}

// draws the bezier curve
void BezierPatch4::draw(Matrix4 C, Frustum F, bool checkCulling)
{
  // load control points
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
    0, 1, 12, 4, &ctrlpoints[0][0][0]);
 /* glMap2f(GL_MAP2_COLOR_4, 0, 1, 4, 4,
    0, 1, 16, 4, &colors[0][0][0]);*/

  // draw curve
  glLoadMatrixd(C.getPointer());
  glEvalMesh2(GL_FILL, 0, 20, 0, 20);
  glPopMatrix();
}

// TODO: finish writing this
void BezierPatch4::computeBoundingSphere(Matrix4 C)
{
}

/*
 * copyAmplitude
 * copies the amplitude and the respective y values of
 * the given patch to the current patch.
 */
void BezierPatch4::copyAmplitude(BezierPatch4 other)
{
  setAmplitude(other.getAmplitude());
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      ctrlpoints[i][j][1] = other.getControlPoint(i, j).get(1);
    }
  }
}


/*
 * setAmplitude
 * 
 * Sets the amplitude and all the y values of the patch to
 * the given amplitude
 */
void BezierPatch4::setAmplitude(double amp)
{
  amplitude = amp;
  // set all of the y values of the current patch to amp
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ctrlpoints[i][j][1] = amp;
}

double BezierPatch4::getAmplitude()
{
  return amplitude;
}

/*
 * join
 *
 * Connects the current patch to three other patches assuming that the calling
 * object is the lower left corner
 */
void BezierPatch4::join(BezierPatch4* top, BezierPatch4* right, double edgeAmp)
{
  // join top edge
  for (int i = 0; i < 4; i++)
  {
    // bottom of the top patch
    Vector3 topPoint = top->getControlPoint(i, 3);
    // connect current patch to bottom of top
    setControlPoint(i, 0, topPoint);
  }

  // join right edge
  if (right)
  {
    for (int i = 1; i <= 3; i++)
    {
      // left of the right patch
      Vector3 rightPoint = right->getControlPoint(0, i);
      // right of current
      Vector3 currPoint = getControlPoint(3,i);

      // set both points y coord to the midpoint
    //  right->setControlPoint(0, i, Vector3(rightPoint.get(0), currPoint.get(1), rightPoint.get(2)));
      setControlPoint(3, i, Vector3(currPoint.get(0), rightPoint.get(1), currPoint.get(2)));

      // C1 version
      // left of the right patch
      rightPoint = right->getControlPoint(1, i);
      // right of left patch
      currPoint = getControlPoint(2, i);
      // float midpoint = (rightPoint.get(1) + currPoint.get(1))/2
      // right->setControlPoint(1, i, Vector3(rightPoint.get(0), currPoint.get(1), rightPoint.get(2)));
      setControlPoint(2, i, Vector3(currPoint.get(0), rightPoint.get(1), currPoint.get(2)));
    }
  }
  else{
    // set the far right edge amplitude
    for (int i = 1; i <= 3; i++)
    {
      Vector3 currPoint = getControlPoint(3, i);
      // set both points y coord to the midpoint
      setControlPoint(3, i, Vector3(currPoint.get(0), edgeAmp, currPoint.get(2)));
    }
  }
}

// connects the current patch to the adjacent one with C1 continuity
void BezierPatch4::joinCont(BezierPatch4* top, BezierPatch4* left)
{
  // join top edge
  for (int i = 0; i < 4; i++)
  {
    // bottom of the top patch
    Vector3 topPoint = top->getControlPoint(i, 2);
    // right of left patch
    Vector3 currPoint = getControlPoint(i,1);
    setControlPoint(i,1, Vector3(currPoint.get(0), topPoint.get(1), currPoint.get(2)));
  }
}
