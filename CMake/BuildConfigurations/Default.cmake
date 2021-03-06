set(MITK_CONFIG_PACKAGES
  OpenMesh
  Qt5
  BLUEBERRY
)

set(MITK_CONFIG_PLUGINS
  org.mitk.gui.qt.mitkworkbench.intro
  org.mitk.gui.qt.datamanager
  org.mitk.gui.qt.stdmultiwidgeteditor
  org.mitk.gui.qt.dicombrowser
  org.mitk.gui.qt.imagenavigator
  org.mitk.gui.qt.measurementtoolbox
  # CustomUI: Must remove this, otherwise CMake will automatically build this plugin
  # org.mitk.gui.qt.properties
  org.mitk.gui.qt.segmentation
  org.mitk.gui.qt.volumevisualization
  org.mitk.planarfigure
  org.mitk.gui.qt.moviemaker
  org.mitk.gui.qt.pointsetinteraction
  org.mitk.gui.qt.remeshing
  org.mitk.gui.qt.viewnavigator
  org.mitk.gui.qt.imagecropper
)
