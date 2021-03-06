#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T15:42:48
# Mingw C:\Qt5\Tools\mingw48_32\i686-w64-mingw32
#-------------------------------------------------
CONFIG   += c++11

QT       += core opengl webkitwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += core widgets

TARGET = runnel_proyect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    runnelcontroller.cpp \
    inputData/googlemap.cpp \
    inputData/datacollector.cpp \
    inputData/tiffdata.cpp \
    inputData/runneldata.cpp \
    inputData/binaryfile.cpp \
    builders/builderterrain.cpp \
    terrain.cpp \
    primitives/point.cpp \
    primitives/triangle.cpp \
    primitives/edge.cpp \
    painters/glwidget.cpp \
    painters/painterterrain.cpp \
    painters/shaderutils.cpp \
    painters/openglutils.cpp \
    painters/shader.cpp \
    algorithmsDrainage/gradientdrainagecallaghanmark.cpp \
    algorithmsDrainage/gradientdrainagehalfpointnetwork.cpp \
    algorithmsDrainage/gradientdrainagenetwork.cpp \
    algorithmsDrainage/peuckerdrainagenetwork.cpp \
    arbol.cpp \
    zhangguilbertalgorithm.cpp \
    algorithmpatron.cpp \
    buildtreecallaghan.cpp \
    buildtreepeucker.cpp


HEADERS  += mainwindow.h \
    runnelcontroller.h \
    inputData/googlemap.h \
    inputData/datacollector.h \
    inputData/webpage.h \
    inputData/tiffdata.h \
    inputData/runneldata.h \
    inputData/binaryfile.h \
    builders/builderterrain.h \
    terrain.h \
    primitives/point.h \
    primitives/triangle.h \
    primitives/edge.h \
    painters/glwidget.h \
    painters/painterterrain.h \
    painters/shaderutils.h \
    painters/openglutils.h \
    painters/shader.h \
    barComplement/barra.h \
    barComplement/ProgressBar.h \
    algorithmsDrainage/gradientdrainagecallaghanmark.h \
    algorithmsDrainage/gradientdrainagehalfpointnetwork.h \
    algorithmsDrainage/gradientdrainagenetwork.h \
    algorithmsDrainage/peuckerdrainagenetwork.h \
    lib/glew/include/GL/glew.h \
    lib/glew/include/GL/glxew.h \
    lib/glew/include/GL/wglew.h \
    lib/glm/detail/_features.hpp \
    lib/glm/detail/_fixes.hpp \
    lib/glm/detail/_literals.hpp \
    lib/glm/detail/_noise.hpp \
    lib/glm/detail/_swizzle.hpp \
    lib/glm/detail/_swizzle_func.hpp \
    lib/glm/detail/_vectorize.hpp \
    lib/glm/detail/func_common.hpp \
    lib/glm/detail/func_exponential.hpp \
    lib/glm/detail/func_geometric.hpp \
    lib/glm/detail/func_integer.hpp \
    lib/glm/detail/func_matrix.hpp \
    lib/glm/detail/func_noise.hpp \
    lib/glm/detail/func_packing.hpp \
    lib/glm/detail/func_trigonometric.hpp \
    lib/glm/detail/func_vector_relational.hpp \
    lib/glm/detail/hint.hpp \
    lib/glm/detail/intrinsic_common.hpp \
    lib/glm/detail/intrinsic_exponential.hpp \
    lib/glm/detail/intrinsic_geometric.hpp \
    lib/glm/detail/intrinsic_integer.hpp \
    lib/glm/detail/intrinsic_matrix.hpp \
    lib/glm/detail/intrinsic_trigonometric.hpp \
    lib/glm/detail/intrinsic_vector_relational.hpp \
    lib/glm/detail/precision.hpp \
    lib/glm/detail/setup.hpp \
    lib/glm/detail/type_float.hpp \
    lib/glm/detail/type_gentype.hpp \
    lib/glm/detail/type_half.hpp \
    lib/glm/detail/type_int.hpp \
    lib/glm/detail/type_mat.hpp \
    lib/glm/detail/type_mat2x2.hpp \
    lib/glm/detail/type_mat2x3.hpp \
    lib/glm/detail/type_mat2x4.hpp \
    lib/glm/detail/type_mat3x2.hpp \
    lib/glm/detail/type_mat3x3.hpp \
    lib/glm/detail/type_mat3x4.hpp \
    lib/glm/detail/type_mat4x2.hpp \
    lib/glm/detail/type_mat4x3.hpp \
    lib/glm/detail/type_mat4x4.hpp \
    lib/glm/detail/type_vec.hpp \
    lib/glm/detail/type_vec1.hpp \
    lib/glm/detail/type_vec2.hpp \
    lib/glm/detail/type_vec3.hpp \
    lib/glm/detail/type_vec4.hpp \
    lib/glm/gtc/constants.hpp \
    lib/glm/gtc/epsilon.hpp \
    lib/glm/gtc/matrix_access.hpp \
    lib/glm/gtc/matrix_integer.hpp \
    lib/glm/gtc/matrix_inverse.hpp \
    lib/glm/gtc/matrix_transform.hpp \
    lib/glm/gtc/noise.hpp \
    lib/glm/gtc/packing.hpp \
    lib/glm/gtc/quaternion.hpp \
    lib/glm/gtc/random.hpp \
    lib/glm/gtc/reciprocal.hpp \
    lib/glm/gtc/type_precision.hpp \
    lib/glm/gtc/type_ptr.hpp \
    lib/glm/gtc/ulp.hpp \
    lib/glm/gtx/associated_min_max.hpp \
    lib/glm/gtx/bit.hpp \
    lib/glm/gtx/closest_point.hpp \
    lib/glm/gtx/color_space.hpp \
    lib/glm/gtx/color_space_YCoCg.hpp \
    lib/glm/gtx/compatibility.hpp \
    lib/glm/gtx/component_wise.hpp \
    lib/glm/gtx/constants.hpp \
    lib/glm/gtx/dual_quaternion.hpp \
    lib/glm/gtx/epsilon.hpp \
    lib/glm/gtx/euler_angles.hpp \
    lib/glm/gtx/extend.hpp \
    lib/glm/gtx/extented_min_max.hpp \
    lib/glm/gtx/fast_exponential.hpp \
    lib/glm/gtx/fast_square_root.hpp \
    lib/glm/gtx/fast_trigonometry.hpp \
    lib/glm/gtx/gradient_paint.hpp \
    lib/glm/gtx/handed_coordinate_space.hpp \
    lib/glm/gtx/inertia.hpp \
    lib/glm/gtx/int_10_10_10_2.hpp \
    lib/glm/gtx/integer.hpp \
    lib/glm/gtx/intersect.hpp \
    lib/glm/gtx/io.hpp \
    lib/glm/gtx/log_base.hpp \
    lib/glm/gtx/matrix_cross_product.hpp \
    lib/glm/gtx/matrix_interpolation.hpp \
    lib/glm/gtx/matrix_major_storage.hpp \
    lib/glm/gtx/matrix_operation.hpp \
    lib/glm/gtx/matrix_query.hpp \
    lib/glm/gtx/matrix_transform_2d.hpp \
    lib/glm/gtx/mixed_product.hpp \
    lib/glm/gtx/multiple.hpp \
    lib/glm/gtx/noise.hpp \
    lib/glm/gtx/norm.hpp \
    lib/glm/gtx/normal.hpp \
    lib/glm/gtx/normalize_dot.hpp \
    lib/glm/gtx/number_precision.hpp \
    lib/glm/gtx/optimum_pow.hpp \
    lib/glm/gtx/orthonormalize.hpp \
    lib/glm/gtx/perpendicular.hpp \
    lib/glm/gtx/polar_coordinates.hpp \
    lib/glm/gtx/projection.hpp \
    lib/glm/gtx/quaternion.hpp \
    lib/glm/gtx/random.hpp \
    lib/glm/gtx/raw_data.hpp \
    lib/glm/gtx/reciprocal.hpp \
    lib/glm/gtx/rotate_normalized_axis.hpp \
    lib/glm/gtx/rotate_vector.hpp \
    lib/glm/gtx/scalar_relational.hpp \
    lib/glm/gtx/simd_mat4.hpp \
    lib/glm/gtx/simd_quat.hpp \
    lib/glm/gtx/simd_vec4.hpp \
    lib/glm/gtx/spline.hpp \
    lib/glm/gtx/std_based_type.hpp \
    lib/glm/gtx/string_cast.hpp \
    lib/glm/gtx/transform.hpp \
    lib/glm/gtx/transform2.hpp \
    lib/glm/gtx/ulp.hpp \
    lib/glm/gtx/unsigned_int.hpp \
    lib/glm/gtx/vec1.hpp \
    lib/glm/gtx/vector_angle.hpp \
    lib/glm/gtx/vector_query.hpp \
    lib/glm/gtx/wrap.hpp \
    lib/glm/common.hpp \
    lib/glm/exponential.hpp \
    lib/glm/ext.hpp \
    lib/glm/fwd.hpp \
    lib/glm/geometric.hpp \
    lib/glm/glm.hpp \
    lib/glm/integer.hpp \
    lib/glm/mat2x2.hpp \
    lib/glm/mat2x3.hpp \
    lib/glm/mat2x4.hpp \
    lib/glm/mat3x2.hpp \
    lib/glm/mat3x3.hpp \
    lib/glm/mat3x4.hpp \
    lib/glm/mat4x2.hpp \
    lib/glm/mat4x3.hpp \
    lib/glm/mat4x4.hpp \
    lib/glm/matrix.hpp \
    lib/glm/packing.hpp \
    lib/glm/trigonometric.hpp \
    lib/glm/vec2.hpp \
    lib/glm/vec3.hpp \
    lib/glm/vec4.hpp \
    lib/glm/vector_relational.hpp \
    arbol.h \
    algorithmpatron.h \
    zhangguilbertalgorithm.h \
    buildtreecallaghan.h \
    buildtreepeucker.h


FORMS    += UI/mainwindow.ui

RESOURCES += \
    RunnelWeb.qrc \
    shader.qrc \
    Icons.qrc

win32: LIBS += -lglew32
unix: LIBS+= -LGLEW

OTHER_FILES += \
    shaders/terrain.vert \
    shaders/terrain.frag \
    shaders/drainage.frag \
    shaders/drainage.vert \
    shaders/gradient.frag \
    shaders/gradient.vert \
    shaders/angle.frag \
    shaders/angle.vert \
    shaders/angle_edge.vert \
    shaders/angle_edge.frag \
    shaders/callaghan.vert \
    shaders/callaghan.frag \
    shaders/arbol_drenaje.vert \
    shaders/arbol_drenaje.frag
