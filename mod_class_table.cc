void ClassTable::Print() {
    Class& cls = Class::Handle();

    // only print object_pool one time
    // bool flag_print_object_pool = true;

    for (intptr_t i = 1; i < top_; i++) {
        if (!HasValidClassAt(i)) {
            continue;
        }
        cls = At(i);

        if (cls.ptr() != nullptr) {
            // auto& cls_funcs = Array::Handle(cls.functions());
            // intptr_t n_funcs = cls_funcs.Length();
            char str_temp[80];
	          auto& funcs = Array::Handle(cls.functions()); 
            intptr_t cls_id = cls.id();

            if (funcs.Length() > 1000) {
                continue;
            }

            // Does the error corresponds to this? Reinit classText for each cls?
            char classText[1000000]="";

            // get the information of Object Pool that corresponds to Classes
            // intptr_t num_cids = NumCids();
            // char temp_num_cids[20];
            // sprintf(temp_num_cids, "%ld", num_cids);
            // strcat(classText, "The number of Class IDs: ");
            // strcat(classText, temp_num_cids);
            // strcat(classText, "\n\n");

            strcat(classText, cls.ToCString());
            strcat(classText, " Class ID: ");
            char temp_cid[20];
            sprintf(temp_cid, "%ld ", cls_id);
            strcat(classText, temp_cid);

            strcat(classText, cls.ToCString());
            strcat(classText, "\nNumber of Class Functions: ");
            sprintf(str_temp, "%ld", funcs.Length());
            strcat(classText, str_temp);
            strcat(classText, "\n");


            for (intptr_t c = 0; c < funcs.Length(); c++) {		    
                auto& func = Function::Handle();    
                func = cls.FunctionFromIndex(c);  	  
                // String& signature = String::Handle();    
                // signature = func.InternalSignature();
                auto& codee = Code::Handle(func.CurrentCode());

                // print object pool
                // if (flag_print_object_pool) {
                //   ObjectPool& object_pool = ObjectPool::Handle(codee.object_pool());
                //   // object_pool.DebugPrint();

                //   intptr_t object_pool_length = object_pool.Length();
                //   for (intptr_t n_object = 0; n_object < object_pool_length; n_object++) {
                    
                //     Object& object = Object::Handle(object_pool.ObjectAt(n_object));
                //     strcat(classText, object.ToCString());
                //   }

                //   flag_print_object_pool = false;
                // }

                // initialize parameter number counters
                // intptr_t num_type_params, num_parent_type_args, num_type_args;
                                    
                if(!func.IsLocalFunction()) {
                    strcat(classText, "\n");
                    strcat(classText, func.ToCString());

                    String& func_source = String::Handle(func.GetSource());
                    strcat(classText, "Function Source: ");
                    strcat(classText, func_source.ToCString());
                    strcat(classText, "\n");
                    
                    char append[120];
                    sprintf(append,"Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));
                    strcat(classText, append);

                    // char temp_functiontype[100];
                    String& func_type = String::Handle(func.InternalSignature());
                    strcat(classText, "Function Signature: ");
                    strcat(classText, func_type.ToCString());
                    strcat(classText, "\n");
                                    
                    // obtain the number of function parameters through "parameter_types()"
                    // intptr_t num_params = func.NumParameters();
                    // intptr_t num_fixed_params = func.num_fixed_parameters();
                    // intptr_t num_optional_params = func.NumOptionalParameters();
                    // char temp_num_params[180];
                    // sprintf(temp_num_params, "NumParameters: %ld, NumFixedParameters: %ld, NumOptionalParameters: %ld", num_params, num_fixed_params, num_optional_params);
                    // strcat(classText, "\n");
                    // strcat(classText, temp_num_params);
                    // obtain the number of function arguments
                    intptr_t num_func_args = func.NumTypeArguments();
                    char temp_num_func_args[20];
                    sprintf(temp_num_func_args, "%ld", num_func_args);
                    strcat(classText, "Number of Function Arguments: ");
                    strcat(classText, temp_num_func_args);
                    strcat(classText, "\n");

                }
                else {
                    auto& parf = Function::Handle();
                    parf=func.parent_function();  
                    String& signParent = String::Handle();
                    signParent = parf.InternalSignature();
                    strcat(classText,"Parent Function Name: ");
                    strcat(classText,parf.ToCString());
                    strcat(classText,"; Parent Signature: ");
                    strcat(classText,signParent.ToCString());
                    // strcat(classText," { \\n\\n          ");
                    char append[120];
                    sprintf(append," Code Offset: _kDartIsolateSnapshotInstructions + 0x%016" PRIxPTR "\\n",static_cast<uintptr_t>(codee.MonomorphicUncheckedEntryPoint()));
                    strcat(classText,append);
                    // strcat(classText,"       \\n       }");
                }
                strcat(classText, " \n");
            }

            strcat(classText, "---------------------\n\n");
            strcat(classText, "\n\n");


            // save the reversed results

            struct stat entry_info;
            int exists = 0;

            if (stat("/data/data/", &entry_info)==0 && S_ISDIR(entry_info.st_mode)) {
                exists=1;
            }

            if(exists==1) {

                pid_t pid = getpid();
                char path[64] = { 0 };
                sprintf(path, "/proc/%d/cmdline", pid);
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
        }
    }
}
