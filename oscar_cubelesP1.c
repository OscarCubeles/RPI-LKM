#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>                 
#include <linux/interrupt.h>            

MODULE_LICENSE("GPL");

static char *argvA[] = {"/home/pi/buttonScripts/buttonA.sh",NULL}; // Path for button A
static char *argvB[] = {"/home/pi/buttonScripts/buttonB.sh",NULL}; // Path for button B
static char *argvC[] = {"/home/pi/buttonScripts/buttonC.sh",NULL}; // Path for button C
static char *argvD[] = {"/home/pi/buttonScripts/buttonD.sh",NULL}; // Path for button D
static char *envp[]= {"HOME=/", NULL};

// Assigning the gpios to variables
static unsigned int gpioLED = 17;            
static unsigned int gpioLED2 = 27;       

static unsigned int gpioButtonA = 5;    
static unsigned int gpioButtonB = 6;    
static unsigned int gpioButtonC = 13;   
static unsigned int gpioButtonD = 19;   

static unsigned int irqNumber1;           
static unsigned int irqNumber2;          
static unsigned int irqNumber3;          
static unsigned int irqNumber4;  

static unsigned int buttonAnumberPresses = 0;  
static unsigned int buttonBnumberPresses = 0;  
static unsigned int buttonCnumberPresses = 0;  
static unsigned int buttonDnumberPresses = 0;  

static bool ledOn = 0;          
static bool led2On = 0;

// Handler functions for the button interrupts
static irq_handler_t  ebbgpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t  ebbgpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t  ebbgpio_irq_handlerC(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t  ebbgpio_irq_handlerD(unsigned int irq, void *dev_id, struct pt_regs *regs);


static int __init ebbgpio_init(void){
   int result = 0;
   printk(KERN_INFO "Phase 1 ASO: Initializing the Kernel Module\n");
   
   if (!gpio_is_valid(gpioLED)||!gpio_is_valid(gpioLED2)){
      printk(KERN_INFO "Phase 1 ASO: invalid LED GPIO\n");
      return -ENODEV;
   }
   
   // LED1
   ledOn = true;
   gpio_request(gpioLED, "sysfs");          
   gpio_direction_output(gpioLED, ledOn);   
   gpio_export(gpioLED, false);             

   //LED2
   led2On = true;
   gpio_request(gpioLED2, "sysfs");            
   gpio_direction_output(gpioLED2, led2On);   
   gpio_export(gpioLED2, false);             
                                             
   //BUTTONA
   gpio_request(gpioButtonA, "sysfs");       
   gpio_direction_input(gpioButtonA);        
   gpio_set_debounce(gpioButtonA, 200);      
   gpio_export(gpioButtonA, false);          
			                                   
   //BUTTONB
   gpio_request(gpioButtonB, "sysfs");       
   gpio_direction_input(gpioButtonB);        
   gpio_set_debounce(gpioButtonB, 200);      
   gpio_export(gpioButtonB, false);          
   
   //BUTTONC                                         
   gpio_request(gpioButtonC, "sysfs");       
   gpio_direction_input(gpioButtonC);        
   gpio_set_debounce(gpioButtonC, 200);      
   gpio_export(gpioButtonC, false);          
            
   //BUTTOND                                         
   gpio_request(gpioButtonD, "sysfs");       
   gpio_direction_input(gpioButtonD);        
   gpio_set_debounce(gpioButtonD, 200);      
   gpio_export(gpioButtonD, false);          
                                            

   // Perform a quick test to see that the button is working as expected on LKM load
   printk(KERN_INFO "Phase 1 ASO: The button state is currently: %d\n", gpio_get_value(gpioButtonA));
   
   irqNumber1 = gpio_to_irq(gpioButtonA);
   printk(KERN_INFO "Phase 1 ASO: The button A is mapped to IRQ: %d\n", irqNumber1);

   irqNumber2 = gpio_to_irq(gpioButtonB);
   printk(KERN_INFO "Phase 1 ASO: The button B is mapped to IRQ: %d\n", irqNumber2);
   
   irqNumber3 = gpio_to_irq(gpioButtonC);
   printk(KERN_INFO "Phase 1 ASO: The button C is mapped to IRQ: %d\n", irqNumber3);
   
   irqNumber4 = gpio_to_irq(gpioButtonD);
   printk(KERN_INFO "Phase 1 ASO: The button D is mapped to IRQ: %d\n", irqNumber4);
   
   // Handlers for interrupts
   result = request_irq(irqNumber1, (irq_handler_t) ebbgpio_irq_handlerA, IRQF_TRIGGER_RISING,"ebb_gpio_handler", NULL);
   result+= request_irq(irqNumber2, (irq_handler_t) ebbgpio_irq_handlerB, IRQF_TRIGGER_RISING,"ebb_gpio_handler", NULL);
   result+=request_irq(irqNumber3, (irq_handler_t) ebbgpio_irq_handlerC, IRQF_TRIGGER_RISING,"ebb_gpio_handler", NULL);
   result+=request_irq(irqNumber4, (irq_handler_t) ebbgpio_irq_handlerD, IRQF_TRIGGER_RISING,"ebb_gpio_handler", NULL);

   printk(KERN_INFO "Phase 1 ASO: The interrupt request result is: %d\n", result);
   return result;
}


static void __exit ebbgpio_exit(void){
   printk(KERN_INFO "Phase 1 ASO: The button state is currently: %d\n", gpio_get_value(gpioButtonD));
   printk(KERN_INFO "Phase 1 ASO: The button A was pressed %d times\n", buttonAnumberPresses);
   printk(KERN_INFO "Phase 1 ASO: The button B was pressed %d times\n", buttonBnumberPresses);
   printk(KERN_INFO "Phase 1 ASO: The button C was pressed %d times\n", buttonCnumberPresses);
   printk(KERN_INFO "Phase 1 ASO: The button D was pressed %d times\n", buttonDnumberPresses);

   gpio_set_value(gpioLED, 0);              
   gpio_unexport(gpioLED);                  
   free_irq(irqNumber1, NULL);               
   free_irq(irqNumber2, NULL);
   free_irq(irqNumber3, NULL);
   free_irq(irqNumber4, NULL);
   gpio_unexport(gpioButtonA);
   gpio_unexport(gpioButtonB);
   gpio_unexport(gpioButtonC);
   gpio_unexport(gpioButtonD);               
   gpio_free(gpioLED);                      
   gpio_free(gpioLED2);
   gpio_free(gpioButtonA);                   
   gpio_free(gpioButtonB);
   gpio_free(gpioButtonC);
   gpio_free(gpioButtonD);
   printk(KERN_INFO "Phase 1 ASO: Goodbye from the LKM!\n");
}

// Handler for button A
static irq_handler_t ebbgpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs){
   ledOn = false;                          
   gpio_set_value(gpioLED, ledOn);          
   printk(KERN_INFO "Phase 1 ASO: Button A state %d\n", gpio_get_value(gpioButtonA));
   printk(KERN_INFO "Phase 1 ASO: The button A has been pressed %d times\n", buttonAnumberPresses);
   buttonAnumberPresses++;                         
   
   call_usermodehelper(argvA[0], argvA, envp, UMH_NO_WAIT);
   return (irq_handler_t) IRQ_HANDLED;      
}

// Handler for button B
static irq_handler_t ebbgpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs){
   ledOn = true;                          
   gpio_set_value(gpioLED, ledOn);          
   
   printk(KERN_INFO "Phase 1 ASO: Button B state %d\n", gpio_get_value(gpioButtonB)); 
   printk(KERN_INFO "Phase 1 ASO: The button B has been pressed %d times\n", buttonBnumberPresses);
   buttonBnumberPresses++;                         
   
   call_usermodehelper(argvB[0], argvB, envp, UMH_NO_WAIT);
   return (irq_handler_t) IRQ_HANDLED;      
}

// Handler for button C
static irq_handler_t ebbgpio_irq_handlerC(unsigned int irq, void *dev_id, struct pt_regs *regs){
   led2On = false;                          
   gpio_set_value(gpioLED2, led2On);          
   
   printk(KERN_INFO "Phase 1 ASO: Button C state %d\n", gpio_get_value(gpioButtonC));
   printk(KERN_INFO "Phase 1 ASO: The button C has been pressed %d times\n", buttonCnumberPresses);   
   buttonCnumberPresses++;                         
   
   call_usermodehelper(argvC[0], argvC, envp, UMH_NO_WAIT);
   return (irq_handler_t) IRQ_HANDLED;      
}

// Handler for button D
static irq_handler_t ebbgpio_irq_handlerD(unsigned int irq, void *dev_id, struct pt_regs *regs){
   led2On = true;                          
   gpio_set_value(gpioLED2, led2On);          
   printk(KERN_INFO "Phase 1 ASO: Button D state %d\n", gpio_get_value(gpioButtonD));
   printk(KERN_INFO "Phase 1 ASO: The button D has been pressed %d times\n", buttonDnumberPresses);
   buttonDnumberPresses++;                  
   
   call_usermodehelper(argvD[0], argvD, envp, UMH_NO_WAIT);
   return (irq_handler_t) IRQ_HANDLED;      
}

module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
