#include "parser.h"

struct sBlock parse(struct sToken *tokens, uint32_t count)
{
    uint32_t i;
    uint32_t j = 0;
    uint32_t command;
    float value;
    struct sBlock block = {0};

    /*Set both feedrate, spindle and delay to negative, this is just to check whether there is a
     * valid F, S or P letter
     */
    block.spindle = -1;
    block.delay = -1.0;

    for(i = 0; i < count; i++)
    {
        value = atof(tokens[i].token + 1);
        command = (uint32_t)value;

        switch(tokens[i].token[0])
        {
        case 'G': case 'g':
            switch(command)
            {
            case 0:
                block.modal[0] = RAPID_POSITIONING;
                block.modal_flags |= BIT_SHIFT(0);
                break;
            case 1:
                block.modal[0] = FEEDRATE_POSITIONING;
                block.modal_flags |= BIT_SHIFT(0);
                break;
            case 90:
                block.modal[3] = ABSOLUTE;
                block.modal_flags |= BIT_SHIFT(3);
                break;
            case 91:
                block.modal[3] = INCREMENTAL;
                block.modal_flags |= BIT_SHIFT(3);
                break;
            case 20:
                block.modal[6] = INCHES;
                block.modal_flags |= BIT_SHIFT(6);
                break;
            case 21:
                block.modal[6] = MILIMETERS;
                block.modal_flags |= BIT_SHIFT(6);
                break;
            case 96:
                block.non_modal[j++] = SET_COORDINATE;
                break;
            case 28:
                block.non_modal[j++] = HOME;
                break;
            case 4:
                block.non_modal[j++] = DWELL;
                break;
                /*ADD MORE G COMMANDS HERE*/
            default:
                //Command not supported
                break;
            }
            break;
        case 'M': case 'm':
            switch(command)
            {
            case 0:
                block.modal[4] = STOP;
                block.modal_flags |= BIT_SHIFT(4);
                break;
            case 1:
                block.modal[4] = STOP;
                block.modal_flags |= BIT_SHIFT(4);
                break;
            case 2:
                block.modal[4] = STOP;
                block.modal_flags |= BIT_SHIFT(4);
                break;
            case 30:
                block.modal[4] = STOP;
                block.modal_flags |= BIT_SHIFT(4);
                break;
            case 60:
                block.modal[4] = STOP;
                block.modal_flags |= BIT_SHIFT(4);
                break;
            case 3:
                block.modal[7] = SPINDLE_CLOCKWISE;
                block.modal_flags |= BIT_SHIFT(7);
                break;
            case 4:
                block.modal[7] = SPINDLE_COUNTERCLOCKWISE;
                block.modal_flags |= BIT_SHIFT(7);
                break;
            case 5:
                block.modal[7] = SPINDLE_STOP;
                block.modal_flags |= BIT_SHIFT(7);
                break;
            case 105:
                block.non_modal[j++] = SEND_TEMPERATURE;
                break;
            case 114:
                block.non_modal[j++] = SEND_POSITION;
                break;
            case 115:
                //block.modal[6] = SET_TEMPERATURE;
                //block.modal_flags |= BIT_SHIFT(6)
                break;
                /*ADD M MORE COMMANDS HERE*/
            default:
                //Command not supported
                break;
            }
           break;
        /*REST OF LETTERS*/
        case 'X': case 'x':
            block.coordinates[0] = value;
            block.axis_flags |= BIT_SHIFT(0);
            break;
        case 'Y': case 'y':
            block.coordinates[1] = value;
            block.axis_flags |= BIT_SHIFT(1);
            break;
        case 'Z': case 'z':
            block.coordinates[2] = value;
            block.axis_flags |= BIT_SHIFT(2);
            break;
        case 'E': case 'e':
            block.coordinates[3] = value;
            block.axis_flags |= BIT_SHIFT(3);
            break;
        case 'F': case 'f':
            block.feedrate = value;
            break;
        case 'S': case 's':
            block.spindle = value;
            break;
        case 'P': case 'p':
            block.delay = value;
            break;
        case '(': case '\r': case '\n':
            break;
        case '*':
            //Checksum
            block.checksum = atoi(tokens[i].token + 1);
            break;
        }
    }

    block.non_modal_count = j;
    return block;
}



