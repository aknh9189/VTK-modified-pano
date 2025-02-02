// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkAxisActor2D
 * @brief   Create an axis with tick marks and labels
 *
 * vtkAxisActor2D creates an axis with tick marks, labels, and/or a title,
 * depending on the particular instance variable settings. vtkAxisActor2D is
 * a 2D actor; that is, it is drawn on the overlay plane and is not
 * occluded by 3D geometry. To use this class, you typically specify two
 * points defining the start and end points of the line (x-y definition using
 * vtkCoordinate class), the number of labels, and the data range
 * (min,max). You can also control what parts of the axis are visible
 * including the line, the tick marks, the labels, and the title.  You can
 * also specify the label format (a printf style format).
 *
 * This class decides what font size to use and how to locate the labels. It
 * also decides how to create reasonable tick marks and labels. The number
 * of labels and the range of values may not match the number specified, but
 * should be close.
 *
 * Labels are drawn on the "right" side of the axis. The "right" side is
 * the side of the axis on the right as you move from Position to Position2.
 * The way the labels and title line up with the axis and tick marks depends on
 * whether the line is considered horizontal or vertical.
 *
 * The vtkActor2D instance variables Position and Position2 are instances of
 * vtkCoordinate. Note that the Position2 is an absolute position in that
 * class (it was by default relative to Position in vtkActor2D).
 *
 * What this means is that you can specify the axis in a variety of coordinate
 * systems. Also, the axis does not have to be either horizontal or vertical.
 * The tick marks are created so that they are perpendicular to the axis.
 *
 * Set the text property/attributes of the title and the labels through the
 * vtkTextProperty objects associated to this actor.
 *
 * @sa
 * vtkCubeAxesActor2D can be used to create axes in world coordinate space.
 *
 * @sa
 * vtkActor2D vtkTextMapper vtkPolyDataMapper2D vtkScalarBarActor
 * vtkCoordinate vtkTextProperty
 */

#ifndef vtkAxisActor2D_h
#define vtkAxisActor2D_h

#include "vtkActor2D.h"
#include "vtkRenderingAnnotationModule.h" // For export macro

#include "vtkNew.h" // for vtkNew

VTK_ABI_NAMESPACE_BEGIN
class vtkPoints;
class vtkPolyData;
class vtkPolyDataMapper2D;
class vtkTextMapper;
class vtkTextProperty;

class VTKRENDERINGANNOTATION_EXPORT vtkAxisActor2D : public vtkActor2D
{
public:
  vtkTypeMacro(vtkAxisActor2D, vtkActor2D);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Instantiate object.
   */
  static vtkAxisActor2D* New();

  ///@{
  /**
   * Specify the position of the first point defining the axis.
   * Note: backward compatibility only, use vtkActor2D's Position instead.
   */
  virtual vtkCoordinate* GetPoint1Coordinate() { return this->GetPositionCoordinate(); }
  virtual void SetPoint1(double x[2]) { this->SetPosition(x); }
  virtual void SetPoint1(double x, double y) { this->SetPosition(x, y); }
  virtual double* GetPoint1() { return this->GetPosition(); }
  ///@}

  ///@{
  /**
   * Specify the position of the second point defining the axis. Note that
   * the order from Point1 to Point2 controls which side the tick marks
   * are drawn on (ticks are drawn on the right, if visible).
   * Note: backward compatibility only, use vtkActor2D's Position2 instead.
   */
  virtual vtkCoordinate* GetPoint2Coordinate() { return this->GetPosition2Coordinate(); }
  virtual void SetPoint2(double x[2]) { this->SetPosition2(x); }
  virtual void SetPoint2(double x, double y) { this->SetPosition2(x, y); }
  virtual double* GetPoint2() { return this->GetPosition2(); }
  ///@}

  ///@{
  /**
   * Specify the (min,max) axis range. This will be used in the generation
   * of labels, if labels are visible.
   */
  vtkSetVector2Macro(Range, double);
  vtkGetVectorMacro(Range, double, 2);
  ///@}

  ///@{
  /**
   * Specify whether this axis should act like a measuring tape (or ruler) with
   * specified major tick spacing. If enabled, the distance between major ticks
   * is controlled by the RulerDistance ivar.
   */
  vtkSetMacro(RulerMode, vtkTypeBool);
  vtkGetMacro(RulerMode, vtkTypeBool);
  vtkBooleanMacro(RulerMode, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Specify the RulerDistance which indicates the spacing of the major ticks.
   * This ivar only has effect when the RulerMode is on.
   */
  vtkSetClampMacro(RulerDistance, double, 0, VTK_FLOAT_MAX);
  vtkGetMacro(RulerDistance, double);
  ///@}

  enum LabelMax
  {
    VTK_MAX_LABELS = 25
  };

  ///@{
  /**
   * Set/Get the number of annotation labels to show. This also controls the
   * number of major ticks shown. Note that this ivar only holds meaning if
   * the RulerMode is off.
   */
  vtkSetClampMacro(NumberOfLabels, int, 2, VTK_MAX_LABELS);
  vtkGetMacro(NumberOfLabels, int);
  ///@}

  ///@{
  /**
   * Get/set the numerical precision to use, default is 2.
   * Precision is only used for scientific and fixed-point notations
   */
  vtkSetClampMacro(Precision, int, 0, VTK_INT_MAX);
  vtkGetMacro(Precision, int);
  ///@}

  ///@{
  /**
   * Get/set number notation to use.
   * Options are:
   *  - Mixed (0, default)
   *  - Scientific (1)
   *  - Fixed-point (2)
   */
  vtkSetClampMacro(Notation, int, 0, 2);
  vtkGetMacro(Notation, int);
  ///@}

  ///@{
  /**
   * Set/Get the format with which to print the labels on the scalar
   * bar.
   */
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);
  ///@}

  ///@{
  /**
   * Set/Get the flag that controls whether the labels and ticks are
   * adjusted for "nice" numerical values to make it easier to read
   * the labels. The adjustment is based in the Range instance variable.
   * Call GetAdjustedRange and GetAdjustedNumberOfLabels to get the adjusted
   * range and number of labels. Note that if RulerMode is on, then the
   * number of labels is a function of the range and ruler distance.
   */
  vtkSetMacro(AdjustLabels, vtkTypeBool);
  vtkGetMacro(AdjustLabels, vtkTypeBool);
  vtkBooleanMacro(AdjustLabels, vtkTypeBool);
  virtual double* GetAdjustedRange()
  {
    this->UpdateAdjustedRange();
    return this->AdjustedRange;
  }
  virtual void GetAdjustedRange(double& _arg1, double& _arg2)
  {
    this->UpdateAdjustedRange();
    _arg1 = this->AdjustedRange[0];
    _arg2 = this->AdjustedRange[1];
  }
  virtual void GetAdjustedRange(double _arg[2]) { this->GetAdjustedRange(_arg[0], _arg[1]); }
  virtual int GetAdjustedNumberOfLabels()
  {
    this->UpdateAdjustedRange();
    return this->AdjustedNumberOfLabels;
  }
  ///@}

  /**
   * Return the positions of ticks along the axis
   */
  vtkPoints* GetTickPositions();

  ///@{
  /**
   * Set/Get the title of the scalar bar actor,
   */
  vtkSetStringMacro(Title);
  vtkGetStringMacro(Title);
  ///@}

  ///@{
  /**
   * Set/Get the title text property.
   */
  virtual void SetTitleTextProperty(vtkTextProperty* p);
  vtkGetObjectMacro(TitleTextProperty, vtkTextProperty);
  ///@}

  ///@{
  /**
   * Set/Get the labels text property.
   */
  virtual void SetLabelTextProperty(vtkTextProperty* p);
  vtkGetObjectMacro(LabelTextProperty, vtkTextProperty);
  ///@}

  ///@{
  /**
   * Set/Get the length of the tick marks (expressed in pixels or display
   * coordinates).
   */
  vtkSetClampMacro(TickLength, int, 0, 100);
  vtkGetMacro(TickLength, int);
  ///@}

  ///@{
  /**
   * Number of minor ticks to be displayed between each tick. Default
   * is 0.
   */
  vtkSetClampMacro(NumberOfMinorTicks, int, 0, 20);
  vtkGetMacro(NumberOfMinorTicks, int);
  ///@}

  ///@{
  /**
   * Set/Get the length of the minor tick marks (expressed in pixels or
   * display coordinates).
   */
  vtkSetClampMacro(MinorTickLength, int, 0, 100);
  vtkGetMacro(MinorTickLength, int);
  ///@}

  ///@{
  /**
   * Set/Get the offset of the labels (expressed in pixels or display
   * coordinates). The offset is the distance of labels from tick marks
   * or other objects.
   */
  vtkSetClampMacro(TickOffset, int, 0, 100);
  vtkGetMacro(TickOffset, int);
  ///@}

  ///@{
  /**
   * Set/Get visibility of the axis line.
   */
  vtkSetMacro(AxisVisibility, vtkTypeBool);
  vtkGetMacro(AxisVisibility, vtkTypeBool);
  vtkBooleanMacro(AxisVisibility, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get visibility of the axis tick marks.
   */
  vtkSetMacro(TickVisibility, vtkTypeBool);
  vtkGetMacro(TickVisibility, vtkTypeBool);
  vtkBooleanMacro(TickVisibility, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get visibility of the axis labels.
   */
  vtkSetMacro(LabelVisibility, vtkTypeBool);
  vtkGetMacro(LabelVisibility, vtkTypeBool);
  vtkBooleanMacro(LabelVisibility, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get visibility of the axis title.
   */
  vtkSetMacro(TitleVisibility, vtkTypeBool);
  vtkGetMacro(TitleVisibility, vtkTypeBool);
  vtkBooleanMacro(TitleVisibility, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get position of the axis title. 0 is at the start of the
   * axis whereas 1 is at the end.
   */
  vtkSetMacro(TitlePosition, double);
  vtkGetMacro(TitlePosition, double);
  ///@}

  ///@{
  /**
   * Set/Get the factor that controls the overall size of the fonts used
   * to label and title the axes. This ivar used in conjunction with
   * the LabelFactor can be used to control font sizes.
   */
  vtkSetClampMacro(FontFactor, double, 0.1, 2.0);
  vtkGetMacro(FontFactor, double);
  ///@}

  ///@{
  /**
   * Set/Get the factor that controls the relative size of the axis labels
   * to the axis title.
   */
  vtkSetClampMacro(LabelFactor, double, 0.1, 2.0);
  vtkGetMacro(LabelFactor, double);
  ///@}

  /**
   * Rebuild the geometry using the provided viewport,
   * and trigger opaque geometry render only if `render` parameter is true.
   * This is used when we need a geometry update (e.g. to draw the grid using tick positions),
   * but the axis should not be rendered.
   */
  int UpdateGeometryAndRenderOpaqueGeometry(vtkViewport* viewport, bool render);

  ///@{
  /**
   * Draw the axis.
   */
  int RenderOverlay(vtkViewport* viewport) override;
  int RenderOpaqueGeometry(vtkViewport* viewport) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport*) override { return 0; }
  ///@}

  /**
   * Does this prop have some translucent polygonal geometry?
   */
  vtkTypeBool HasTranslucentPolygonalGeometry() override;

  /**
   * Release any graphics resources that are being consumed by this actor.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow*) override;

  /**
   * This method computes the range of the axis given an input range.
   * It also computes the number of tick marks given a suggested number.
   * (The number of tick marks includes end ticks as well.)
   * The number of tick marks computed (in conjunction with the output
   * range) will yield "nice" tick values. For example, if the input range
   * is (0.25,96.7) and the number of ticks requested is 10, the output range
   * will be (0,100) with the number of computed ticks to 11 to yield tick
   * values of (0,10,20,...,100).
   */
  static void ComputeRange(
    double inRange[2], double outRange[2], int inNumTicks, int& outNumTicks, double& interval);

  /**
   * General method to computes font size from a representative size on the
   * viewport (given by size[2]). The method returns the font size (in points)
   * and the string height/width (in pixels). It also sets the font size of the
   * instance of vtkTextMapper provided. The factor is used when you're trying
   * to create text of different size-factor (it is usually = 1 but you can
   * adjust the font size by making factor larger or smaller).
   */
  static int SetMultipleFontSize(vtkViewport* viewport, vtkTextMapper** textMappers,
    int nbOfMappers, int* targetSize, double factor, int* stringSize);

  ///@{
  /**
   * Specify whether to size the fonts relative to the viewport or relative to
   * length of the axis. By default, fonts are resized relative to the viewport.
   */
  vtkSetMacro(SizeFontRelativeToAxis, vtkTypeBool);
  vtkGetMacro(SizeFontRelativeToAxis, vtkTypeBool);
  vtkBooleanMacro(SizeFontRelativeToAxis, vtkTypeBool);
  ///@}

  ///@{
  /**
   * By default the AxisActor controls the font size of the axis label.  If this
   * option is set to true, it will instead use whatever font size is set in the
   * vtkTextProperty, allowing external control of the axis size.
   */
  vtkSetMacro(UseFontSizeFromProperty, vtkTypeBool);
  vtkGetMacro(UseFontSizeFromProperty, vtkTypeBool);
  vtkBooleanMacro(UseFontSizeFromProperty, vtkTypeBool);
  ///@}

  /**
   * Shallow copy of an axis actor. Overloads the virtual vtkProp method.
   */
  void ShallowCopy(vtkProp* prop) override;

protected:
  vtkAxisActor2D();
  ~vtkAxisActor2D() override;

  vtkTextProperty* TitleTextProperty;
  vtkTextProperty* LabelTextProperty;

  char* Title;
  char* LabelFormat;
  double Range[2] = { 0., 1. };
  double TitlePosition = 0.5;
  vtkTypeBool RulerMode = 0;
  double RulerDistance = 1.;
  int NumberOfLabels = 5;
  vtkTypeBool AdjustLabels = 1;
  double FontFactor = 1.;
  double LabelFactor = 0.75;
  int TickLength = 5;
  int MinorTickLength = 3;
  int TickOffset = 2;
  int NumberOfMinorTicks = 0;

  double AdjustedRange[2];
  int AdjustedNumberOfLabels = 5;
  int NumberOfLabelsBuilt = 0;
  vtkNew<vtkPoints> TicksStartPos;

  int Notation = 0;
  int Precision = 2;

  vtkTypeBool AxisVisibility = 1;
  vtkTypeBool TickVisibility = 1;
  vtkTypeBool LabelVisibility = 1;
  vtkTypeBool TitleVisibility = 1;

  int LastPosition[2] = { 0, 0 };
  int LastPosition2[2] = { 0, 0 };

  int LastSize[2] = { 0, 0 };
  int LastMaxLabelSize[2] = { 0, 0 };

  int SizeFontRelativeToAxis = 0;
  vtkTypeBool UseFontSizeFromProperty = 0;

  virtual void BuildAxis(vtkViewport* viewport);
  static double ComputeStringOffset(double width, double height, double theta);
  static void SetOffsetPosition(double xTick[3], double theta, int stringWidth, int stringHeight,
    int offset, vtkActor2D* actor);
  virtual void UpdateAdjustedRange();

  vtkTextMapper* TitleMapper;
  vtkActor2D* TitleActor;

  vtkTextMapper** LabelMappers;
  vtkActor2D** LabelActors;

  vtkNew<vtkPolyData> Axis;
  vtkNew<vtkPolyDataMapper2D> AxisMapper;
  vtkNew<vtkActor2D> AxisActor;

  vtkTimeStamp AdjustedRangeBuildTime;
  vtkTimeStamp BuildTime;

private:
  vtkAxisActor2D(const vtkAxisActor2D&) = delete;
  void operator=(const vtkAxisActor2D&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
