#include "vm/class_table.h"

#include <limits>
#include <memory>

#include "platform/atomic.h"
#include "vm/flags.h"
#include "vm/growable_array.h"
#include "vm/heap/heap.h"
#include "vm/object.h"
#include "vm/object_graph.h"
#include "vm/raw_object.h"
#include "vm/visitor.h"


// The original version of ClassTable::Print() function in class_table.c
void ClassTable::Print() {
    Class& cls = Class::Handle();
    String& cls_name = String::Handle();
    String& lib_name = String::Handle();

    char classText[100000]="";

    for (intptr_t i = 1; i < top_; i++) {
        if (!HasValidClassAt(i)) {
            continue;
        }
        cls = At(i);
        auto& cls_funcs = Array::Handle(cls.functions());
        if (cls.ptr() != nullptr) {
            char str_temp[100];
            cls_name = cls.Name();

            sprintf(str_temp, "Class Name: %s, Number of Class Functions: %ld", cls_name.ToCString(), cls_functions.Length());

            // const Library& libr = Library::Handle(cls.library());
            // if (!libr.IsNull()) {
            //     lib_name = libr.url();
            //     sprintf(str_temp, "Class Name: %s, Library Name: %s", cls_name.ToCString(), lib_name.ToCString());
            // }
            // else {
            //     sprintf(str_temp, "Class Name: %s, Number of Class functions: %d", cls_name.ToCString(), cls_funcs.Length());
            // }

            
            // sprintf(str_temp, "Class Name: %s, Library Name: %s", cls_name.ToCString(), lib_name.ToCString());
            
            strcat(classText, str_temp);
            strcat(classText, "\n\n");
        }
    }
    // save the reversed results

    struct stat entry_info;	  
    int exists = 0;	  

    if (stat("/data/data/", &entry_info)==0 && S_ISDIR(entry_info.st_mode)){		  
        exists=1;	  
    }	  	  	  

    if(exists==1){

        pid_t pid = getpid();
        char path[64] = { 0 };
        sprintf(path, "/proc/%d/cmdline", pid);
        FILE *cmdline = fopen(path, "r");		solutions = [
  {
    "managed": False,
    "name": "src/flutter",
    "url": "git@github.com:<your_name_here>/engine.git",
    "custom_deps": {},
    "deps_file": "DEPS",
    "safesync_url": "",
  },
]

        if (cmdline) {
            char chm[264] = { 0 };
            char pat[264] = { 0 };
            char application_id[64] = { 0 };
            fread(application_id, sizeof(application_id), 1, cmdline);
            sprintf(pat, "/data/data/%s/dump.dart", application_id);
            
            do {
                FILE *f = fopen(pat, "a+");
                fprintf(f, "%s",classText);
                fflush(f);
                fclose(f);
                sprintf(chm,"/data/data/%s",application_id);
                chmod(chm, S_IRWXU|S_IRWXG|S_IRWXO);
                chmod(pat, S_IRWXU|S_IRWXG|S_IRWXO);
            } while (0);
            
            fclose(cmdline);
        }	
    }

    if(exists==0){			  	   		
        char pat[264] = { 0 };		
        sprintf(pat, "%s/Documents/dump.dart", getenv("HOME"));   
        OS::PrintErr("reFlutter dump file: %s",pat);   

        do { FILE *f = fopen(pat, "a+");   
            fprintf(f, "%s",classText);   
            fflush(f);   
            fclose(f);   	  
        } while (0);         	  
    }
}

void ClassTable::Print() {
    Class& cls = Class::Handle();
    String& name = String::Handle();

    for (intptr_t i = 1; i < top_; i++) {
        if (!HasValidClassAt(i)) {
        continue;
        }
        cls = At(i);
        if (cls.ptr() != nullptr) {
            name = cls.Name();

            auto& funcs = Array::Handle(cls.functions());   

            if (funcs.Length()>1000) {    
                continue;    
            }	
            char classText[100000]=""; 	  


            String& supname = String::Handle();  	  
            name = cls.Name();	
            strcat(classText,cls.ToCString());  	  
            Class& supcls = Class::Handle();    
            supcls = cls.SuperClass();  	  
            if (!supcls.IsNull()) {		 
                supname = supcls.Name();		  
                strcat(classText," extends ");		 
                strcat(classText,supname.ToCString()); 	
            }		  
            const auto& interfaces = Array::Handle(cls.interfaces());	
            auto& interface = Instance::Handle();		  
            for (intptr_t in = 0;in < interfaces.Length(); in++) {	
                interface^=interfaces.At(in);	
                if(in==0) {
                    strcat(classText," implements ");
                } 	  
                if(in>0) {
                    strcat(classText," , ");  
                }		
                strcat(classText,interface.ToCString());	
            }
            strcat(classText," {\\n");	
            const auto& fields = Array::Handle(cls.fields());   	  
            auto& field = Field::Handle();	
            auto& fieldType = AbstractType::Handle(); 	  
            String& fieldTypeName = String::Handle();	
            String& finame = String::Handle();		  
            Instance& instance2 = Instance::Handle();		  
            for (intptr_t f = 0; f < fields.Length(); f++) {    
                field ^= fields.At(f);	
                finame = field.name();	
                fieldType = field.type();	
                fieldTypeName = fieldType.Name();	
                strcat(classText,"  ");		  
                strcat(classText,fieldTypeName.ToCString()); 	
                strcat(classText," ");	strcat(classText,finame.ToCString()); 		  
                if(field.is_static()) {			
                    instance2 ^= field.StaticValue();			
                    strcat(classText," = ");			  
                    strcat(classText,instance2.ToCString());			
                    strcat(classText," ;\\n");  
                } 	  
                
                else {	  
                    strcat(classText," = ");	  
                    strcat(classText," nonstatic;\\n");  
                    }	
            }  	  
            for (intptr_t c = 0; c < funcs.Length(); c++) {		    
                auto& func = Function::Handle();    
                func = cls.FunctionFromIndex(c);  	  
                String& signature = String::Handle();    
                signature = func.InternalSignature();
                auto& codee = Code::Handle(func.CurrentCode());	  	  
                
                if(!func.IsLocalFunction()) {	
                    strcat(classText," \\n  ");	
                    strcat(classText,func.ToCString());	
                    strcat(classText," ");    
                    strcat(classText,signature.ToCString());		  
                    strcat(classText," { \\n\\n              ");	  
                    char append[70];	  
                    sprintf(append," Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));	  
                    strcat(classText,append);		  
                    strcat(classText,"       \\n       }\\n");		  
                } 
                else {		  
                    auto& parf = Function::Handle();	
                    parf=func.parent_function();		  
                    String& signParent = String::Handle();   		  
                    signParent = parf.InternalSignature();			  
                    strcat(classText," \\n  ");			  
                    strcat(classText,parf.ToCString());	
                    strcat(classText," ");	
                    strcat(classText,signParent.ToCString());		  
                    strcat(classText," { \\n\\n          "); 	  
                    char append[80];	  
                    sprintf(append," Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));	  
                    strcat(classText,append);		  
                    strcat(classText,"       \\n       }");		
                }	

                // get function arguments
                // the error (Stack dump aborted because InitialRegisterCheck failed.)
                // occured since the initialization of Array "argument_types"
                // intptr_t num_type_parameters = func.NumTypeParameters();
                // intptr_t num_parent_type_arguments = func.NumParentTypeArguments();
                // intptr_t num_type_arguments = func.NumTypeArguments();

                intptr_t num_type_parameters = func.NumTypeParameters();
                char temp_append[30];
                sprintf(temp_append, "NumTypeParameters: %d", num_type_parameters);
                strcat(classText, temp_append);
                
                const auto& argument_types = Array::Handle(func.parameter_types());
                
                for (intptr_t c_arg = 0; c_arg < argument_types.Length(); c_arg++) {	
                    auto& argument_type = AbstractType::Handle();
                    argument_type = func.ParameterTypeAt(c_arg);
                    // argument_type ^= argument_types.At(c_arg);
                    strcat(classText, argument_type.ToCString());
                    strcat(classText, "; ");
                }
                strcat(classText, "\\n\\n");

            }		  	  
            strcat(classText," \\n      }\\n\\n");	  	  
            const Library& libr = Library::Handle(cls.library());
            if (!libr.IsNull()) {  
                auto& owner_class = Class::Handle(); 
                owner_class = libr.toplevel_class();   
                auto& funcsTopLevel = Array::Handle(owner_class.functions());   
                char pushTmp[1000];   
                String& owner_name = String::Handle();   
                owner_name = libr.url();   
                sprintf(pushTmp,"\'%s\',",owner_name.ToCString());  
                if (funcsTopLevel.Length()>0&&strstr(pushArr, pushTmp) == NULL) {  
                    strcat(pushArr,pushTmp);   
                    strcat(classText,"Library:"); 
                    strcat(classText,pushTmp); 
                    strcat(classText," {\\n");         
                    for (intptr_t c = 0; c < funcsTopLevel.Length(); c++) {      
                        auto& func = Function::Handle();    
                        func = owner_class.FunctionFromIndex(c);  	  
                        String& signature = String::Handle();    	  
                        signature = func.InternalSignature();	  
                        auto& codee = Code::Handle(func.CurrentCode());	   
                        if(!func.IsLocalFunction()) {		  
                            strcat(classText," \\n  ");	
                            strcat(classText,func.ToCString());	
                            strcat(classText," ");    
                            strcat(classText,signature.ToCString());		  
                            strcat(classText," { \\n\\n              ");	  
                            char append[70];	  
                            sprintf(append," Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));	  
                            strcat(classText,append);		  
                            strcat(classText,"       \\n       }\\n");		  
                        } 
                        else {		  
                            auto& parf = Function::Handle();	
                            parf=func.parent_function();		  
                            String& signParent = String::Handle();   		  
                            signParent = parf.InternalSignature();			  
                            strcat(classText," \\n  ");			  
                            strcat(classText,parf.ToCString());	
                            strcat(classText," ");	
                            strcat(classText,signParent.ToCString());		  
                            strcat(classText," { \\n\\n          "); 	  
                            char append[80];	  
                            sprintf(append," Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));	  
                            strcat(classText,append);		  
                            strcat(classText,"       \\n       }\\n");		
                        }

                        intptr_t num_type_parameters = func.NumTypeParameters();
                        char temp_append[30];
                        sprintf(temp_append, "NumTypeParameters: %d", num_type_parameters);
                        strcat(classText, temp_append);

                    }             
                    strcat(classText," \\n      }\\n\\n");
                }
            }	  
            struct stat entry_info;	  
            int exists = 0;	  

            if (stat("/data/data/", &entry_info)==0 && S_ISDIR(entry_info.st_mode)){		  
                exists=1;	  
            }	  	  	  
        
            if(exists==1){		  
  
                FILE *cmdline = fopen(path, "r");		

                if (cmdline) {			  	    
                    char chm[264] = { 0 };		
                    char pat[264] = { 0 };        
                    char application_id[64] = { 0 };		        
                    fread(application_id, sizeof(application_id), 1, cmdline);		
                    sprintf(pat, "/data/data/%s/dump.dart", application_id);		        
                    
                    do { 
                        FILE *f = fopen(pat, "a+");   
                        fprintf(f, "%s",classText);   
                        fflush(f);   
                        fclose(f);   
                        sprintf(chm,"/data/data/%s",application_id);  
                        chmod(chm, S_IRWXU|S_IRWXG|S_IRWXO);  
                        chmod(pat, S_IRWXU|S_IRWXG|S_IRWXO);	  
                    } while (0);        
                    
                    fclose(cmdline);    
                }	  
            }	  	  	  	  	  	  	  	  	  	  	  	  	  
            
            if(exists==0){			  	   		
                char pat[264] = { 0 };		
                sprintf(pat, "%s/Documents/dump.dart", getenv("HOME"));   
                OS::PrintErr("reFlutter dump file: %s",pat);   

                do { FILE *f = fopen(pat, "a+");   
                    fprintf(f, "%s",classText);   
                    fflush(f);   
                    fclose(f);   	  
                } while (0);         	  
            }

        }

    }
}