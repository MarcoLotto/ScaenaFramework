/**********************************
 * SCAENA FRAMEWORK
 * Author: Marco AndrÈs Lotto
 * License: MIT - 2016
 **********************************/

#import "GameViewController.h"
#import <CoreMotion/CoreMotion.h>

#include "myWindow.h"
#include "GraphicDevice.h"
#include "VideoConfiguration.h"
#include "TouchEvent.h"
#include "WindowConnector.h"

#define IOS_DOWNSCALE_FACTOR 2.0 // Factor de escala de la pantalla (IOS hace downscale de la pantalla)

@interface GameViewController () {
    
}
@property (strong, nonatomic) GLKBaseEffect *effect;
@property (strong, nonatomic) CMMotionManager *motionManager;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

WindowConnector* windowConnector = NULL;
GLuint renderbuffer;
EAGLContext *context;
float acelerometerX, acelerometerY, acelerometerZ;

- (void)viewDidLoad
{
    // Ajustamos la resolucion de la pantalla para que quede igual que los bounds de touch
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    [self.view setBounds:CGRectMake(0, 0, screenBounds.size.width, screenBounds.size.height)];
    self.view.contentScaleFactor = IOS_DOWNSCALE_FACTOR;  // Determina el factor de escala de la pantalla (forzar a un valor por si hay dispositivos con pantalla con mas resolucion)
    
    [super viewDidLoad];
    [self.view setMultipleTouchEnabled:YES];
    [self setupGL];
    [self initScanaEngine];
    [self initCoreMotion];
}

- (void)initCoreMotion
{
    self.motionManager = [[CMMotionManager alloc] init];
    self.motionManager.deviceMotionUpdateInterval = 20.0;  // No nos interesa usar este sensor
    self.motionManager.gyroUpdateInterval = 20.0;          // No nos interesa usar este sensor
    self.motionManager.accelerometerUpdateInterval = 0.1;  // Modificador de velocidad del acelerometro (en segundos)
    [self.motionManager startDeviceMotionUpdates];
    
    // Definimos que queremos data del acelerometro
    [self.motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue]
            withHandler:^(CMAccelerometerData  *accelerometerData, NSError *error) {
                [self outputAccelertionData:accelerometerData.acceleration];
    }];
    
    /*
    // Se sensa el giroscopio
    [self.motionManager startGyroUpdatesToQueue:[NSOperationQueue currentQueue]
            withHandler:^(CMGyroData *gyroData, NSError *error) {
                [self outputRotationData:gyroData.rotationRate];
    }];
     */
}

- (void)dealloc
{    

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)createDirectoryWithPath:(NSString*)path usingName:(NSString*)name
{
    NSError *error;
    NSString *dataPath = [path stringByAppendingPathComponent: name];
    if (![[NSFileManager defaultManager] fileExistsAtPath:dataPath])
        [[NSFileManager defaultManager] createDirectoryAtPath:dataPath withIntermediateDirectories:NO attributes:nil error:&error];
}

- (void)initScanaEngine
{
    // Encuentro cual es el path de la carpeta de recursos del dispositivo
    NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
    WindowConnector::setBaseApplicationPath(std::string([resourcePath UTF8String]) + "/");
    
    // Encontramos la carpeta donde guardaremos la configuracion de la aplicacion
    NSString* path = [NSSearchPathForDirectoriesInDomains( NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    WindowConnector::setApplicationConfigurationPath(std::string([path UTF8String]) + "/");
    
    // Creamos (si no existen) los directorios de configuracion
    [self createDirectoryWithPath: path usingName: @"/ProgramData"];
    
    // Consigo el tamaño de la pantalla
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenWidthInTouchPoints = screenRect.size.width;
    CGFloat screenHeightInTouchPoints = screenRect.size.height;
    
    CGFloat screenWidthInPixels = screenWidthInTouchPoints * IOS_DOWNSCALE_FACTOR;
    CGFloat screenHeightInPixels = screenHeightInTouchPoints * IOS_DOWNSCALE_FACTOR;
    
    //Creo el conector de Scaena con la ventana
    windowConnector = new myWindow();
    windowConnector->OnInit();

    // Indicamos el tamaño de la ventana y cargamos recursos
    windowConnector->OnResize((int) screenWidthInPixels, (int) screenHeightInPixels);
    
    // Cambiamos el tamaño del area de accion de la UI (por el downscale de la imagen que se hace en IOS)
    windowConnector->OnScreenUIDimensionsResize((int) screenWidthInTouchPoints, (int) screenHeightInTouchPoints);
}

- (void)setupGL
{
    // Verifico que version de OpenGLES voy a usar y le indico a Scaena
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!context) {
        // No esta disponible openglES3 en el dispositivo, pruebo con openGLES2
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context) {
            NSLog(@"No se pudo crear un contexto valido para OpenGL, su dispositivo no parece ser compatible");
            exit(1);
        }
        NSLog(@"OPENGLES VERSION 2");
        GraphicDevice::bindApi(OPENGLES_2_API);
    }
    else{
        NSLog(@"OPENGLES VERSION 3");
        GraphicDevice::bindApi(OPENGLES_3_API);
    }
    
    // Activamos el contexto de OpenGL
    if (![EAGLContext setCurrentContext:context]) {
        NSLog(@"No se pudo activar el contexto de EAGLContext");
        exit(1);
    }
    
    // Next we need to set up the renderbuffer.
    glGenRenderbuffers(1, &renderbuffer);
    
    // Bind the renderbuffer to the GL_RENDERBUFFER.
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    
    // Allocate storage for the renderbuffer.
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable: (CAEAGLLayer*)self.view.layer];
    
    // Finally, we need to set up the framebuffer.
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    
    // Bind the framebuffer to the GL_FRAMEBUFFER.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    // Attach our renderbuffer to the framebuffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    
    // Le informo a Scaena cual es el default frame buffer para dibujar a la pantalla
    GraphicDevice::setDefaultFrameBuffer(framebuffer);
}

- (void)tearDownGL
{

}

- (void)update
{
    // Le indicamos a Scaena que haga update
    windowConnector->OnUpdate();
    
    // Le indicamos a Scaena que renderice
    windowConnector->OnRender();
}

void appSwapBuffers(){
    // Volvemos a bindear el render buffer antes de presentar la pantalla
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    
    // En IOS se utiliza un RenderBuffer para el render a la pantalla,
    // hacemos con esto el pasaje final de la imagen a la pantalla
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

// Cuando se detecte un cambio en el acelerometro, se ejecuta este metodo
-(void)outputAccelertionData:(CMAcceleration)acceleration
{
    // Conseguimos los datos del sensor
    acelerometerX = -acceleration.x;
    acelerometerY = -acceleration.y;
    acelerometerZ = -acceleration.z;
    
    // Actualizamos los datos de los sensores
    SensorsData sensorsData;
    sensorsData.setAcelerometer(vec3(acelerometerX, acelerometerY, acelerometerZ));
    windowConnector->OnSensorsUpdate(sensorsData);
    //NSLog(@"Acelerometer=%f, %f, %f", acelerometerX, acelerometerY, acelerometerZ);
}

// Cuando se detecte un cambio en el giroscopio, se ejecuta este metodo (si esta habilitado en initCoreMotion)
-(void)outputRotationData:(CMRotationRate)rotation
{
    NSLog(@"Gyroscope=%f, %f, %f", rotation.x, rotation.y, rotation.z);
}

 - (void)processTouches: (NSSet*)touches : (TouchType)touchType{
     TouchEvent touchEvent(touchType, WindowConnector::GetOSCurrentTime());
     int i = 0;
     for (UITouch* touch in touches)
     {
         CGPoint point = [touch locationInView:self.view];
         int touchId = touch.hash;
         touchEvent.addTouchPoint(new TouchPoint(point.x, point.y, touchId, true));
         
         // Si es el primer toque, tambien lo informe como toque del mouse (para UI)
         if(i == 0){
             if(touchType == TouchType::began)
                 windowConnector->OnMouseDown(0, point.x, point.y);
             else if(touchType == TouchType::ended)
                 windowConnector->OnMouseUp(0, point.x, point.y);
         }
         i++;
     }
     // Asignamos los datos de los sensores al evento touch
     touchEvent.getSensorsData().setAcelerometer(vec3(acelerometerX, acelerometerY, acelerometerZ));
     // Terminamos
     windowConnector->OnTouchEvent(&touchEvent);
 }
 
 - (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
     [self processTouches : touches : TouchType::began];
 }
 - (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
     [self processTouches : touches : TouchType::moved];
 }
 - (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
     [self processTouches : touches : TouchType::ended];
 }
 - (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
     [self processTouches : touches : TouchType::cancelled];
 }
 


@end
