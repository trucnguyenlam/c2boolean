

def main(staticvar, totalvar):
    ret = ''

    ret += 'unsigned int TRACK_ADDRESS_tmp[%s];\n' % (totalvar - staticvar-1)

    for i in range(0, staticvar):
        ret += 'TRACK_ADDRESS[%s] = %s;\n' % (i, i)


    ret += 'num_malloc = %s;\n' % staticvar


    for i in range(staticvar, totalvar-1):
        ret += 'TRACK_ADDRESS[%s] = TRACK_ADDRESS_tmp[%s];\n' % (i, i-staticvar)
        ret += 'assume((TRACK_ADDRESS[%s] <= INTERVAL) & (TRACK_ADDRESS[%s] > TRACK_ADDRESS[%s]));\n' % (i, i, i-1)

    ret += 'TRACK_ADDRESS[%s] = INTERVAL + 1;\n' % (totalvar-1)

    return ret

if __name__ == '__main__':
    print main(17, 31)